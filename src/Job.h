#ifndef BAKUP_AGENT_JOB_H
#define BAKUP_AGENT_JOB_H

#include <Request.h>
#include <Command.h>
#include <Response.h>
#include <ResponseBuilder.h>
#include <Debug.h>
#include <future>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <curl/curl.h>

using namespace std;

class Job
{
    private:
        // Struct to hold the job information
        command_t job;

        // Copy of the debug class to print information
        Debug &debug;

        // Hold the job confirmation URL
        string jobConfirmationURL;

        // Hold the client Id
        string clientId;

        // Hold the authentication token
        string apiToken;

        // Store the job's output
        string jobOutput;

    public:
        // Constructor that must be given a reference to debug object and the job to be done
        Job(Debug &debug, command_t &job, string jobConfirmationURL, string clientId, string authToken, bool autoExecute = true);

        // Process the commands in the job
        int process(bool autoReportResults = true);

        // Report the results to Bakup
        bool reportResults(int retryCounter, int maxRetry);

        // Handle printing errors
        bool handleError(string &httpResponse, cpr::Error error);
};


#endif //BAKUP_AGENT_JOB_H
