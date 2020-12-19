#ifndef BAKUP_AGENT_RESPONSE_H
#define BAKUP_AGENT_RESPONSE_H

#include <string>
#include <cpr/cpr.h>
#include <cpr/error.h>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace std;
using namespace rapidjson;

class Response
{
    private:
        // Client Id
        const string clientId;

        // Auth Token
        const string authToken;

        // URL to access
        const string url;

        // Store errors
        cpr::Error error;

        // Response from bakup
        string response;

        // Post data to a URL
        int apiPostData(cpr::Header &headers, string &postData, string &postResponse);

    public:
        // Construct the class
        Response(string url, string clientId, string authToken);

        // Send job confirmation information back to bakup
        int postJobConfirmation(string &postData);

        // Get response data from server in case of error
        string getResponse();

        // Get error status
        cpr::Error getError();

        // Get error code
        cpr::ErrorCode getErrorCode();

        // Get error message
        string getErrorMessage();
};

#endif //BAKUP_AGENT_RESPONSE_H
