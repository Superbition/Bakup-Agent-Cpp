#ifndef BAKUP_AGENT_RESPONSEBUILDER_H
#define BAKUP_AGENT_RESPONSEBUILDER_H

#define SUCCESS_CODE 0
#define ERROR_CODE_JOB_FAIL 1
#define ERROR_CODE_JSON_FAIL 2
#define ERROR_CODE_SSL_FAIL 3
#define ERROR_CODE_WRITE_PIPE_FAIL 4
#define ERROR_CODE_READ_PIPE_FAIL 5

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

        // Add an error code
        void addErrorCode(int errorCode);

        // Add an error message
        void addErrorMessage(string errorMessage);

        // Add a send attempt int
        void addSendAttempt(int sendAttempt);

        // Add commands from a vector using the commandOutput data structure
        void addCommandOutputs(vector<commandOutput> &commandsOutput);

        // Add job ID
        void addJobId(string id);

        // Add the job type
        void addJobType(string jobType);
};


#endif //BAKUP_AGENT_RESPONSEBUILDER_H
