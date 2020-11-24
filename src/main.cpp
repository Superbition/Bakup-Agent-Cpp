#include <main.h>

using namespace std;
using namespace rapidjson;

// The main function that handles the program loop
int main(int argc, char *argv[])
{
    // Initialise the agent class
    Agent agent;

    // Change user identity to given user ID
    int uid = stoi(agent.getUserID());
    setuid(uid);

    // Get the program loop wait time
    int waitTime = agent.getWaitTime();

    // Set the program to non-debug mode
    Debug debug(false, agent);

    // If there is a command line argument
    if(argc == 2)
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
    while(true)
    {
        // Test Bakup request
        Request job(agent.getBakupRequestURL(), agent.getAuthToken());
        int jobStatusCode = job.getBakupJob();

        // A vector of strings to hold job commands
        vector<string> jobs;

        // Check if the request was successful
        if(jobStatusCode == 200)
        {
            debug.print("Successful backup job request");

            // Parse the response from Bakup to get the job list
            jobs = job.getVectoredResponse();

            // If debug mode is enabled
            if(debug.getDebugMode())
            {
                if(!jobs.empty())
                {
                    debug.print("Commands received:");
                    for(string& command: jobs)
                    {
                        debug.print(command);
                    }
                }
                else
                {
                    debug.print("No commands were found in the job");
                }
            }
        }
        // Else the request was not successful
        else
        {
            // Get HTTP error
            string failedResponse = job.getResponse();

            // If the HTTP error is not empty
            if(failedResponse.length() != 0)
            {
                // Print the HTTP error response
                debug.print("Backup job request failed, printing error:");
                debug.print(failedResponse);
            }

            // If the error code is none zero
            if (static_cast<bool>(job.getError()))
            {
                // Get the libcurl error for printing
                string error = curl_easy_strerror(static_cast<CURLcode>(job.getErrorCode()));
                int errorCode = static_cast<CURLcode>(job.getErrorCode());

                // Print the libcurl errors
                debug.print("libcurl error: " + to_string(errorCode) + ": " + error);
            }

            // Get the CPR error
            string cprError = job.getErrorMessage();

            // Check if the error exists
            if(cprError.length() != 0)
            {
                // Print the CPR error
                debug.print("CPR error: " + cprError);
            }
        }

        if(!jobs.empty())
        {
            // Create a string buffer and writer for creating a JSON string
            StringBuffer s;
            Writer<StringBuffer> writer(s);
            writer.StartArray();

            for(int i = 0; i < jobs.size(); i++)
            {
                // Start a new object within the outer JSON object
                writer.StartObject();

                // Set up the command and working directory
                Command command(jobs[i]);
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
                if(commandStatusCode != EXIT_SUCCESS)
                {
                    break;
                }
            }

            // End the JSON string
            writer.EndArray();

            // Convert the JSON object to a string
            string jobStatusString = s.GetString();

            // Print the output for debug
            debug.print(jobStatusString);

            // Get the URL to send the job result to
            const string jobConfirmationUrl = agent.getBakupJobConfirmationURL();

            // Send the job response to Bakup
            Response response(jobConfirmationUrl, agent.getAuthToken());
            int jobConfStatus = response.postJobConfirmation(jobStatusString);

            if(jobConfStatus != 200)
            {
                debug.print("Sending job confirmation failed");

                // Get HTTP error
                string failedResponse = response.getResponse();

                // If the HTTP error is not empty
                if(failedResponse.length() != 0)
                {
                    // Print the HTTP error response
                    debug.print("Backup job request failed, printing error:");
                    debug.print(failedResponse);
                }

                // If the error code is none zero
                if (static_cast<bool>(response.getError()))
                {
                    // Get the libcurl error for printing
                    string error = curl_easy_strerror(static_cast<CURLcode>(response.getErrorCode()));
                    int errorCode = static_cast<CURLcode>(response.getErrorCode());

                    // Print the libcurl errors
                    debug.print("libcurl error: " + to_string(errorCode) + ": " + error);
                }

                // Get the CPR error
                string cprError = response.getErrorMessage();

                // Check if the error exists
                if(cprError.length() != 0)
                {
                    // Print the CPR error
                    debug.print("CPR error: " + cprError);
                }
            }
            else
            {
                debug.print("Successfully sent job confirmation");

                // Print the outcome of the job response
                debug.print("Job confirmation response: " + to_string(jobConfStatus));
            }
        }

        // Wait before asking for another job
        sleep(waitTime);
    }

    return EXIT_SUCCESS;
}
