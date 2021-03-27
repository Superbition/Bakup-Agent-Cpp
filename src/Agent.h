#ifndef BAKUP_AGENT_AGENT_H
#define BAKUP_AGENT_AGENT_H
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <sys/utsname.h>
#include <Request.h>
#include <Response.h>
#include <ResponseBuilder.h>
#include <Debug.h>
#include <Job.h>
#include <SSLChecker.h>
#include <curl/curl.h>
#include <cpr/cpr.h>
#ifdef TESTING
#include <gtest/gtest.h>
#endif

using namespace std;

class Agent
{
    private:
        // Folder for configuration files
        const string configDirectory = "/etc/opt/bakupagent";

        // OS information file
        const string osReleaseFile = "/etc/os-release";

        // Location of the client ID
        const string clientIdLocation = configDirectory + "/CLIENT_ID";

        // Get the client ID
        string clientId = this->readFile(clientIdLocation);

        // Location of the authentication token
        const string apiTokenLocation = configDirectory + "/API_TOKEN";

        // Get the authentication token
        string apiToken = this->readFile(apiTokenLocation);

        // Location of the user ID to run the program as
        const string userIDLocation = configDirectory + "/USER_ID";

        // Get the user ID
        string userID = this->readFile(userIDLocation);

        // Location of the first initialisation ping file
        string initialisedLocation = configDirectory + "/.INITIALISED";

        // Host URL
        const string host = "bakup.io";

        // Base URL
        const string baseUrl = "/api/agent";

        // Api version base url
        const string apiVersionBaseUrl = "/v";

        // Api version to be used in URLs
        const string apiVersion = "1";

        // Version of the agent
        const string agentVersion = "v4.0";

        // Program loop wait time in seconds
        const int pollTime = 60;

        // Time to wait before retrying failed request
        const int retryTime = 10;

        // Store job commands
        vector<command_t> jobs;

#ifdef TESTING
        // Friend class for testing skipPollTimes
        FRIEND_TEST(AgentTest, SkipPollTime);
#endif
        // Output from commands ran
        string commandsOutput;

    public:
        // constructor
        Agent();

        // Copy constructor
        Agent(const Agent &obj);

        // Skip the next wait time on the main loop
        bool skipNextPollTime = false;

        // Read a file to a string
        string readFile(const string &fileLocation);

        // Get the client id
        string getClientId();

        // Get the auth token
        string getApiToken();

        // Get the user ID
        string getUserID();

        // Generate the base url
        string getBaseURL();

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

        // Check if the first ping has been sent to backup, if not send it
        bool checkFirstRunAndPing(Debug &debug);

        // Change the effective user id of the program
        bool changeEUID(int uid, command_t &job);
};

#endif //BAKUP_AGENT_AGENT_H
