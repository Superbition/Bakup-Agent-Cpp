#ifndef BAKUP_AGENT_AGENT_H
#define BAKUP_AGENT_AGENT_H
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <Request.h>
#include <Debug.h>
#include <Command.h>
#include <Response.h>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace std;

class Agent
{
    private:
        // Folder for configuration files
        const string configDirectory = "/etc/opt/bakupagent";

        // Location of the authentication token
        const string authorisationLocation = configDirectory + "/AUTH_TOKEN";

        // Get the authentication token
        const string authToken = this->readFile(authorisationLocation);

        // Location of the user ID to run the program as
        const string userIDLocation = configDirectory + "/USER_ID";

        // Get the user ID
        const string userID = this->readFile(userIDLocation);

        // Host URL
        const string host = "https://bakup.io";

        // Base URL
        const string baseUrl = "/api";

        // Api version base url
        const string apiVersionBaseUrl = "/v";

        // Api version to be used in URLs
        const string apiVersion = "1";

        // Url to check for bakups
        const string bakupRequestUrl = "/bakup/request";

        // Url for job confirmations
        const string bakupJobConfirmationUrl = "/bakup/confirm";

        // Version of the agent
        const string agentVersion = "v1.0";

        // Program loop wait time in seconds
        const int pollTime = 60;

        // Store job commands
        vector<string> commands;

        // Output from commands ran
        string commandsOutput;

    public:
        // Read a file to a string
        string readFile(const string &fileLocation);

        // Get the auth token
        string getAuthToken();

        // Get the user ID
        string getUserID();

        // Generate a bakup request url
        string getBakupRequestURL();

        // Generate a bakup job confirmation url
        string getBakupJobConfirmationURL();

        // Get the agent's version number
        string getAgentVersion();

        // Return the wait time for the main program loop
        int getWaitTime();

        // Function for handling web related errors
        bool handleError(Debug &debug, string httpResponse, cpr::Error error);

        // Get job from Bakup
        bool getJob(Debug &debug);

        // Run commands from Bakup
        bool runCommands(Debug &debug);

        // Report results back to bakup
        bool reportResults(Debug &debug);

        // Reset job related variables
        bool resetJob(Debug &debug);
};

#endif //BAKUP_AGENT_AGENT_H
