#ifndef BAKUP_AGENT_RESPONSE_H
#define BAKUP_AGENT_RESPONSE_H

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

class Response {
private:
    // Auth Token
    const string authToken;

    // URL to access
    const string url;

    // Response from bakup
    string response;

    // Post data to a URL
    int apiPostData(cpr::Header &headers, string &postData, string &postResponse);

public:
    // Construct the class
    Response(string url, string authToken);

    // Send job confirmation information back to bakup
    int postJobConfirmation(string &postData, string &postResponse);

};

#endif //BAKUP_AGENT_RESPONSE_H
