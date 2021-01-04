#ifndef BAKUP_AGENT_AGENT_H
#define BAKUP_AGENT_AGENT_H
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <Request.h>
#include <Debug.h>
#include <Job.h>
#include <curl/curl.h>
#include <cpr/cpr.h>

using namespace std;

class Agent
{
    private:
        // Folder for configuration files
        const string configDirectory = "/etc/opt/bakupagent";

        // Location of the client ID
        const string clientIdLocation = configDirectory + "/CLIENT_ID";

        // Get the client ID
        string clientId = this->readFile(clientIdLocation);

        // Location of the authentication token
        const string authorisationLocation = configDirectory + "/AUTH_TOKEN";

        // Get the authentication token
        string authToken = this->readFile(authorisationLocation);

        // Location of the user ID to run the program as
        const string userIDLocation = configDirectory + "/USER_ID";

        // Get the user ID
        string userID = this->readFile(userIDLocation);

        // Host URL
        const string host = "https://bakup.io";

        // Base URL
        const string baseUrl = "/api/agent";

        // Api version base url
        const string apiVersionBaseUrl = "/v";

        // Api version to be used in URLs
        const string apiVersion = "1";

        // Url to check for bakups
        const string bakupRequestUrl = "/job/request";

        // Url for job confirmations
        const string bakupJobConfirmationUrl = "/job/confirm";

        // Version of the agent
        const string agentVersion = "v2.0";

        // Program loop wait time in seconds
        const int pollTime = 60;

        // Time to wait before retrying failed request
        const int retryTime = 10;

        // Store job commands
        vector<command_t> jobs;

        // Output from commands ran
        string commandsOutput;

    public:
        // constructor
        Agent();

        // Copy constructor
        Agent(const Agent &obj);

        // Read a file to a string
        string readFile(const string &fileLocation);

        // Get the client id
        string getClientId();

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

        // Get the command output
        string getCommandOutput();

        // Return the wait time for the main program loop
        int getWaitTime();

        // Return the retry wait time
        int getRetryTime();

        // Get job from Bakup
        bool getJob(Debug &debug, int retryCounter, int retryMaxCount);

        // Handle printing error
        bool handleError(Debug &debug, string httpResponse, cpr::Error error);

        // Reset job related variables
        bool resetJob(Debug &debug);

        // Get number of jobs
        int getNumberOfJobs();

        // Process the jobs
        bool processJobs(Debug &debug);

        // Re-read the authentication files in to memory
        void refreshAgentCredentials(Debug &debug);
};

#endif //BAKUP_AGENT_AGENT_H
