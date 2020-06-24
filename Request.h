#ifndef BAKUP_AGENT_REQUEST_H
#define BAKUP_AGENT_REQUEST_H

#include <string>
#include "cpr/include/cpr/cpr.h"
#include "cpr/include/cpr/parameters.h"
#include "cpr/include/cpr/cprtypes.h"
#include <vector>
#include "include/rapidjson/include/rapidjson/document.h"
#include "include/rapidjson/include/rapidjson/writer.h"
#include "include/rapidjson/include/rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;

class Request {
    // Auth Token
    const string authToken;

    // URL to access
    const string url;

    // Response from bakup
    string response;

    // Converted json response to vector
    vector<string> vectorResponse;

public:
    // Construct the class
    Request(string url, string authToken);

    // Check bakup for any jobs
    int getBakupJob(string &content);

    // Send an API Get Request and return the JSON response
    int apiGetRequest(cpr::Parameters &parameters, cpr::Header &headers, string &content);

    // Parse a job response to a vector
    static vector<string> parseBakupResponse(string &jsonString);
};


#endif //BAKUP_AGENT_REQUEST_H
