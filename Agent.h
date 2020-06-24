#ifndef BAKUP_AGENT_AGENT_H
#define BAKUP_AGENT_AGENT_H
#include <string>

using namespace std;

class Agent {
    // Folder for configuration files
    const string configDirectory = "/etc/bakupagent";

    // Working directory for storing temp data
    const string workingDirectory = "/tmp";

    // Store the location of the config file
    const string authorisationLocation = configDirectory + "/AUTH_TOKEN";

    // Get the config file contents
    const string authToken = this->readFile(authorisationLocation);

    // Store the host URL
    const string host = "localhost";

    // Store the base URL
    const string baseUrl = "/api";

    // Store the api version base url
    const string apiVersionBaseUrl = "/v";

    // Store the api version to be used in URLs
    const string apiVersion = "1";

    // Store the url to check for bakups
    const string bakupRequestUrl = "/bakup/request";

    // Store the url for job confirmations
    const string bakupJobConfirmationUrl = "/bakup/confirm";

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
};


#endif //BAKUP_AGENT_AGENT_H
