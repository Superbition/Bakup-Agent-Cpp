#include "main.h"
#include "Debug.h"
#include "Agent.h"

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

// Process the given command and write the output to output
int processCommand(const char *command, string mainDirectory, string workingDirectory, string &output)
{
    // Switch to the temporary working directory
    chdir(workingDirectory.c_str());

    // An array to hold the output of the stream from the command process
    array<char, 128> buffer{};

    // Open a stream in read mode using the supplied command
    auto pipe = popen(command, "r");

    // If the stream fails to open, return an error
    if(!pipe)
    {
        output = "Popen failed to open a stream";
        return -1;
    }
    else // Otherwise, the stream can be used successfully
    {
        // Read from stream until EOF is found
        while(!feof(pipe))
        {
            // If the read data is not null
            if(fgets(buffer.data(), 128, pipe) != nullptr)
            {
                // Write the line of output to the output string
                output += buffer.data();
            }
        }
    }

    // Close the pipe and read the status code
    auto statusCode = pclose(pipe);

    // Switch back to the main directory
    chdir(mainDirectory.c_str());

    // Return the status code of the command
    return statusCode;
}

int apiPostData(const string &url, cpr::Header &headers, string &postData, string &response)
{
    // Make the post to Bakup
    cpr::Response r = cpr::Post(cpr::Url{url},
                      cpr::Header{headers},
                      cpr::Body{postData});

    // Set the returned content
    response = r.text;

    // return the status code
    return r.status_code;
}

int postJobConfirmation(const string &url, const string &authorisationToken, string &postData, string &response)
{
    // Add the authorisation token to the headers
    cpr::Header headers = cpr::Header{{"Authorization", authorisationToken}, {"Content-Type", "text/json"}};

    // Variable to store response data inside
    string responseData;

    // Post the data
    int responseCode = apiPostData(url, headers, postData, responseData);

    // Set the response data that is returned from Bakup
    response = responseData;

    // return response code
    return responseCode;
}

// The main function that handles the program loop
int main(int argc, char* argv[])
{
    // Initialise the agent class
    Agent agent;

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

    // Test Bakup request
    string url = agent.getBakupRequestURL();
    string bakupContent;
    int statusCode = requestBakupUpdate(url, agent.getAuthToken(), bakupContent);

    if (statusCode >= 400)
    {
        debug.Print("Error code " + to_string(statusCode));
        debug.Print(bakupContent);
    }
    else
    {
        debug.Print("Successful request");
        debug.Print(bakupContent);
    }

    debug.Print("Parsing bakup response");

    string jobString = bakupContent;

    // Convert the JSON string in to a vector for looping through
    vector<string> jobs = parseBakupResponse(jobString);

    // Get the current working directory
    string cwd = filesystem::current_path();

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

    for (int i = 0; i < jobs.size(); i++)
    {
        writer.StartObject();
        // The command needs to have stderr redirected to stdout so that both can be captured
        string command = jobs[i] + " 2>&1";

        // A string to store the result in
        string result;

        // Run the command and get the exit code
        int commandStatusCode = processCommand(command.c_str(), cwd, absoluteWorkingDir, result);

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

    string jobStatusString = s.GetString();
    debug.Print(jobStatusString);
    const string jobConfirmationUrl = agent.getBakupJobConfirmationURL();
    string jobResponse;
    int jobConfStatus = postJobConfirmation(jobConfirmationUrl, agent.getAuthToken(), jobStatusString, jobResponse);

    debug.Print(to_string(jobConfStatus));
    debug.Print(jobResponse);

    // Remove the temporary directory
    rmdir(absoluteWorkingDir.c_str());

    return EXIT_SUCCESS;
}
