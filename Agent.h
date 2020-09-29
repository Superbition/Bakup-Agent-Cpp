#ifndef BAKUP_AGENT_AGENT_H
#define BAKUP_AGENT_AGENT_H
#include <string>

using namespace std;

class Agent
{
    private:
        // Folder for configuration files
        const string configDirectory = "/etc/bakupagent";

        // Working directory for storing temp data
        const string workingDirectory = "/tmp";

        // Location of the config file
        const string authorisationLocation = configDirectory + "/AUTH_TOKEN";

        // Get the config file contents
        const string authToken = this->readFile(authorisationLocation);

        // Host URL
        const string host = "localhost";

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

        // Program loop wait time
        const int waitTime = 60 * 1000;

    public:
        // Read a file to a string
        string readFile(const string &fileLocation);

        // Get the auth token
        string getAuthToken();

        // Generate a bakup request url
        string getBakupRequestURL();

        // Generate a bakup job confirmation url
        string getBakupJobConfirmationURL();

        // Return the working directory
        string getWorkingDirectory();

        // Return the wait time for the main program loop
        int getWaitTime();
};

#endif //BAKUP_AGENT_AGENT_H
