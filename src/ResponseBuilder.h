#ifndef BAKUP_AGENT_RESPONSEBUILDER_H
#define BAKUP_AGENT_RESPONSEBUILDER_H

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <vector>

using namespace std;

// Struct to hold the output of a job
struct commandOutput
{
    string command;
    int statusCode;
    string result;
};

class ResponseBuilder
{
    private:
        // String buffer to hold the in progress json object
        rapidjson::StringBuffer json;

        // Store a writer object for use
        rapidjson::Writer<rapidjson::StringBuffer> *writer = new rapidjson::Writer<rapidjson::StringBuffer>(this->json);

    public:
        ResponseBuilder();

        // Finish the JSON object and return string
        string build();

        // Add a send attempt int
        void addSendAttempt(int sendAttempt);

        // Add commands from a vector using the commandOutput data structure
        void addCommandOutputs(vector<commandOutput> &commandsOutput);
};


#endif //BAKUP_AGENT_RESPONSEBUILDER_H
