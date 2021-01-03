#ifndef BAKUP_AGENT_REQUEST_H
#define BAKUP_AGENT_REQUEST_H

#include <string>
#include <cpr/cpr.h>
#include <cpr/error.h>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <Debug.h>
#include <ResponseBuilder.h>

using namespace std;
using namespace rapidjson;

// Data structure to hold command information
struct command_t
{
    string id = "";
    int targetExecutionTime = 0;
    vector<string> commands;
};

class Request
{
    private:
        // Client Id
        const string clientId;

        // Auth Token
        const string authToken;

        // URL to access
        const string url;

        // Response from bakup
        string response;

        // Converted json response to vector
        vector<command_t> vectorResponse;

        // Store error codes
        cpr::Error error;

        // Debug class for printing
        Debug debug;

        // Send an API Get Request and return the JSON response
        int apiGetRequest(cpr::Parameters &parameters, cpr::Header &headers, string &content);

        // Parse a job response to a vector
        vector<command_t> parseBakupResponse(string &jsonString);

        // Track if the JSON received was valid
        bool JsonValid = true;

        // Hold the raw JSON
        string json;

    public:
        // Construct the class
        Request(string url, string clientId, string authToken, Debug &debug);

        // Check bakup for any jobs
        int getBakupJob();

        // Get the response
        string getResponse();

        // Get the vector response
        vector<command_t> getVectoredResponse();

        // Get error status
        cpr::Error getError();

        // Get error code
        cpr::ErrorCode getErrorCode();

        // Get error message
        string getErrorMessage();

        // Get if json was valid
        bool isJsonValid();

        // Get raw json
        string getJson();
};

#endif //BAKUP_AGENT_REQUEST_H
