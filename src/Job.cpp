#include "Job.h"

Job::Job(Debug &debug, command_t &job, string baseUrl, string clientId, string apiToken, bool autoExecute) :
        debug(ref(debug)),
        job(std::move(job)),
        baseURL(std::move(baseUrl)),
        apiToken(std::move(apiToken)),
        clientId(std::move(clientId))
{
    if(autoExecute)
    {
        // Start the job processing
        this->process();
    }
}

int Job::process(bool autoReportResults)
{
    // Start a new command instance
    Command command(debug);

    // Store the exit status of the overall job
    int exitStatus = 0;

    // Set up the command execution environment
    if(!command.setupEnvironment())
    {
        // Build error response and send to Bakup.io
        ResponseBuilder responseBuilder;
        responseBuilder.addSendAttempt(1);
        responseBuilder.addJobId(job.id);
        responseBuilder.addErrorCode(ERROR_CODE_JOB_FAIL);
        responseBuilder.addErrorMessage("Could not open a child process to run the job");
        this->jobOutput = responseBuilder.build();

        // Set the exit status
        exitStatus = ERROR_CODE_JOB_FAIL;

        // Report the results to Bakup
        this->reportResults(1, 5);

        // Kill the bash child
        command.~Command();

        return exitStatus;
    }

    if(!checkShellReady(command, 1, 2))
    {
        // Build error response and send to Bakup.io
        ResponseBuilder responseBuilder;
        responseBuilder.addSendAttempt(1);
        responseBuilder.addJobId(job.id);
        responseBuilder.addErrorCode(ERROR_CODE_WRITE_PIPE_FAIL);
        responseBuilder.addErrorMessage("Could not contact child process");
        this->jobOutput = responseBuilder.build();

        // Set the exit status
        exitStatus = ERROR_CODE_JOB_FAIL;

        // Report the results to Bakup
        this->reportResults(1, 5);

        // Kill the bash child
        command.~Command();

        return exitStatus;
    }

    // Check the job vector isn't empty
    if(!empty(this->job.commands))
    {
        // If the desired execution time is in the future, sleep until then
        // Bakup should return jobs chronologically, so jobs won't execute late
        if(this->job.targetExecutionTime > time(NULL))
        {
            debug.info("Waiting " + to_string(this->job.targetExecutionTime - time(NULL)) + " seconds until desired execution time of command");
            sleep(this->job.targetExecutionTime - time(NULL));
        }

        // Create a responseBuilder object to hold the output of this job
        ResponseBuilder responseBuilder;

        // Add the send attempt element
        responseBuilder.addSendAttempt(1);

        // Add the Job ID
        responseBuilder.addJobId(job.id);

        // Hold all the command's output in this vector
        vector<commandOutput> commandsOutput;

        // Loop through all of the jobs
        for(int i = 0; i < this->job.commands.size(); i++)
        {
            // Store this commands output
            commandOutput tempCommandOutput;

            // Run the command and get the exit code and result
            pair<string, exit_status_t> result = command.runCommand(this->job.commands[i]);
            string output = result.first;
            exit_status_t commandStatusCode = result.second;

            // Store the information in the struct
            tempCommandOutput.command = job.commands[i];
            tempCommandOutput.statusCode = commandStatusCode;
            tempCommandOutput.result.append(output);

            commandsOutput.push_back(tempCommandOutput);

            // If the command didn't execute properly
            if(commandStatusCode != EXIT_SUCCESS)
            {
                exitStatus = 1;
                int statusCode = ERROR_CODE_JOB_FAIL;

                // Check if the exit status can be converted to a ResponseBuilder error
                switch(commandStatusCode)
                {
                    case exit_status_t::ES_READ_FAILED:
                        statusCode = ERROR_CODE_READ_PIPE_FAIL;
                        break;
                    case exit_status_t::ES_WRITE_FAILED:
                        statusCode = ERROR_CODE_WRITE_PIPE_FAIL;
                        break;
                    case exit_status_t::ES_EXIT_STATUS_NOT_FOUND:
                        statusCode = ERROR_CODE_JOB_FAIL;
                        break;
                }

                // Add the error code
                responseBuilder.addErrorCode(statusCode);

                // Add the latest job output as the error
                responseBuilder.addErrorMessage(job.commands[i]);
                break;
            }
        }

        // If the command ran successfully
        if(exitStatus == EXIT_SUCCESS)
        {
            // Add the success error code
            responseBuilder.addErrorCode(SUCCESS_CODE);
        }

        // Add the vector of outputs
        responseBuilder.addCommandOutputs(commandsOutput);

        // Build the response and get the string
        this->jobOutput = responseBuilder.build();

        // If the autoReportResults is set
        if(autoReportResults)
        {
            // Report the results to bakup
            this->reportResults(1, 5);
        }
    }

    // Kill the bash child
    command.~Command();

    return exitStatus;
}

bool Job::reportResults(int retryCounter, int maxRetry)
{
    // Check if the maximum number of retries has been met
    if(retryCounter <= maxRetry)
    {
        // Build the response object to send command output back to Bakup
        Response response(this->baseURL, this->clientId, this->apiToken);

        // Execute and get the status
        int jobConfStatus = response.postJobConfirmation(this->jobOutput);
        string jobConfOutput = response.getResponse();

        // If the job failed
        if(jobConfStatus != 200)
        {
            // Handle reporting the error to Bakup
            this->handleError(jobConfOutput, response.getError());

            // Check to see if the request failed due to SSL and safely report it
            SSLChecker sslChecker(response.getErrorCode());
            if(!sslChecker.checkSSLValid())
            {
                debug.error("Sending HTTP-safe SSL error report to Bakup");

                // Build the error to send to Bakup
                ResponseBuilder responseBuilder;
                responseBuilder.addErrorCode(ERROR_CODE_SSL_FAIL);
                responseBuilder.addErrorMessage("SSL Failed with: " + response.getErrorMessage());
                string sslErrorMessage = responseBuilder.build();

                // Send to Bakup without apiToken due to plaintext protocol
                Response sslResponse(this->baseURL, this->clientId, this->apiToken);
                sslResponse.postSSLError(sslErrorMessage);
            }

            // Retry sending the result to Bakup
            this->reportResults(++retryCounter, maxRetry);
            return false;
        }
        else
        {
            debug.success("Successfully sent job confirmation");
            debug.info("Job confirmation response: " + to_string(jobConfStatus) + ": " + jobConfOutput);
        }
    }

    return true;
}

bool Job::handleError(string &httpResponse, cpr::Error error)
{
    this->debug.error("Sending job confirmation failed");

    // If the HTTP error is not empty
    if(httpResponse.length() != 0)
    {
        // Print the HTTP error response
        this->debug.error("Backup job request failed, printing error:");
        this->debug.error(httpResponse);
    }

    // If the error code is none zero
    if(static_cast<bool>(error))
    {
        // Get the libcurl error for printing
        string errorMessage = curl_easy_strerror(static_cast<CURLcode>(error.code));
        int errorCode = static_cast<CURLcode>(error.code);

        // Print the libcurl errors
        this->debug.error("libcurl error: " + to_string(errorCode) + ": " + errorMessage);
    }

    // Check if cpr error exists
    if(error.message.length() != 0)
    {
        // Print the CPR error
        this->debug.error("CPR error: " + error.message);
    }

    return true;
}

bool Job::checkShellReady(Command &command, int retryAmount, int secondsToWait)
{
    // Check that the shell is ready to be written to by sending an echo and checking the exist status
    auto [output, exitStatus] = command.runCommand("echo");

    // Check for a write pipe error
    if(exitStatus == exit_status_t::ES_WRITE_FAILED)
    {
        // If there are still retries left
        if(retryAmount > 0)
        {
            // Sleep until next attempt
            sleep(secondsToWait);

            // Try again
            return this->checkShellReady(command, --retryAmount, secondsToWait);
        }
        else // Else, return false, bash environment not ready
        {
            return false;
        }
    }
    else // Else, no write error
    {
        // Shell is ready
        return true;
    }
}
