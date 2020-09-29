#include "main.h"
#include "Debug.h"
#include "Agent.h"
#include "Request.h"
#include "Command.h"
#include "Response.h"

using namespace std;
using namespace rapidjson;

// Given a reference to a 20 byte char array, populate it with a datetime
void currentDateTime(char* dateTime)
{
    // Setup the specialised time variable for holding the current datetime
    time_t rawTime;

    // Struct to hold localised raw time codes
    struct tm * timeInfo;

    // The buffer to store the formatted datetime value in, should not be more that 20 bytes
    char buffer [20];

    // Populate datetime and struct with localised time
    time (&rawTime);
    timeInfo = localtime (&rawTime);

    // Format the raw time in to the given format string
    strftime(buffer, 20, "%Y-%m-%dT%H:%M:%S", timeInfo);

    // Copy the contents of the buffer in to the dateTime parameter
    strncpy(dateTime, buffer, 20);
}

// The main function that handles the program loop
int main(int argc, char* argv[])
{
    // Initialise the agent class
    Agent agent;

    // Get the program loop wait time
    int waitTime = agent.getWaitTime();

    // Set the program to non-debug mode
    Debug debug(false);

    // If there is a command line argument
    if (argc == 2)
    {
        // If the -d flag is passed
        if(strcmp(argv[1], "-d") == 0)
        {
            // Set the debug class to true, to log to command line
            debug.setDebugMode(true);
        }
    }
    // If there is more than one command line argument
    else if(argc > 2)
    {
        return EXIT_FAILURE;
    }

    // Main program loop
    while(True) {

        // Test Bakup request
        Request job(agent.getBakupRequestURL(), agent.getAuthToken());
        int jobStatusCode = job.getBakupJob();

        // A vector of strings to hold job commands
        vector<string> jobs;

        if (jobStatusCode == 200) {
            debug.Print("Successful backup job request");
            jobs = job.getVectoredResponse();
        } else {
            debug.Print("Backup job request failed");
            string failedResponse = job.getResponse();
            debug.Print(failedResponse);
        }

        if (!jobs.empty()) {
            char timestamp[20];
            // Generate a name for a temp directory to work in
            currentDateTime(timestamp);
            string workingDir = string("/temp") + timestamp;
            string absoluteWorkingDir = agent.getWorkingDirectory() + workingDir;
            mkdir(absoluteWorkingDir.c_str(), S_IRWXU);

            // Create a string buffer and writer for creating a JSON string
            StringBuffer s;
            Writer<StringBuffer> writer(s);
            writer.StartArray();

            for (int i = 0; i < jobs.size(); i++) {
                // Start a new object within the outer JSON object
                writer.StartObject();

                // Set up the command and working directory
                Command command(jobs[i], absoluteWorkingDir);
                // Run the command and get the exit code
                int commandStatusCode = command.process();
                // Get the output of the command
                string result = command.getOutput();

                // Write the output and status of the command to the JSON object
                writer.Key("command");
                writer.String(jobs[i].c_str());
                writer.Key("status_code");
                writer.Int(commandStatusCode);
                writer.Key("result");
                writer.String(result.c_str());
                writer.EndObject();

                // If the command didn't execute properly
                if (commandStatusCode != EXIT_SUCCESS) {
                    break;
                }
            }

            // End the JSON string
            writer.EndArray();

            string jobStatusString = s.GetString();
            debug.Print(jobStatusString);
            const string jobConfirmationUrl = agent.getBakupJobConfirmationURL();
            string postJobResponse;
            Response response(jobConfirmationUrl, agent.getAuthToken());
            int jobConfStatus = response.postJobConfirmation(jobStatusString, postJobResponse);

            debug.Print(to_string(jobConfStatus));
            debug.Print(postJobResponse);

            // Remove the temporary directory
            rmdir(absoluteWorkingDir.c_str());
        }

        sleep(waitTime);
    }

    return EXIT_SUCCESS;
}
