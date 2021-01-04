#include <Agent.h>

Agent::Agent()
{
    this->apiToken = this->readFile(this->apiTokenLocation);
    this->userID = this->readFile(this->userIDLocation);
};

Agent::Agent(const Agent &obj)
{
    // Set the temp values in the new agent object
    this->clientId = obj.clientId;
    this->apiToken = obj.apiToken;
    this->userID = obj.userID;
    this->jobs = obj.jobs;
    this->commandsOutput = obj.commandsOutput;
}

std::string Agent::readFile(const std::string &fileLocation)
{
    // Open the file stream using the given file location
    std::ifstream fileStream(fileLocation);

    if(fileStream)
    {
        // The place holder string to hold the contents of the file
        std::string fileText;

        // Seek to the end of the file
        fileStream.seekg(0, std::ios::end);

        // Reserve memory for the string using the new position in the file stream
        fileText.reserve(fileStream.tellg());

        // Move the position back to the start of the file
        fileStream.seekg(0, std::ios::beg);

        // Assign the file text using the iterator from the file stream
        fileText.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

        // Remove trailing new line
        if (!fileText.empty() && fileText[fileText.length()-1] == '\n')
        {
            fileText.erase(fileText.length()-1);
        }

        return fileText;
    }
    else
    {
        cerr << "Agent could not access file: " << fileLocation << endl;
        cerr << "Error reason: " << strerror(errno) << endl;
        cerr << "Try running the install script again to generate required files. If this does not work, please contact support" << endl;
        std::exit(1);
    }
}

string Agent::getBakupRequestURL()
{
    return this->host + this->baseUrl + this->apiVersionBaseUrl + this->apiVersion + this->bakupRequestUrl;
}

string Agent::getClientId()
{
    return this->clientId;
}

string Agent::getApiToken()
{
    return this->apiToken;
}

string Agent::getUserID()
{
    return this->userID;
}

string Agent::getBakupJobConfirmationURL()
{
    return this->host + this->baseUrl + this->apiVersionBaseUrl + this->apiVersion + this->bakupJobConfirmationUrl;
}

string Agent::getAgentVersion() {
    return this->agentVersion;
}

string Agent::getCommandOutput() {
    return this->commandsOutput;
}

int Agent::getWaitTime()
{
    return this->pollTime;
}

int Agent::getRetryTime() {
    return this->retryTime;
}

bool Agent::getJob(Debug &debug, int retryCounter, int retryMaxCount)
{
    // Get a job from Bakup
    Request job(this->getBakupRequestURL(), this->getClientId(), this->getApiToken(), debug);
    int jobStatusCode = job.getBakupJob();

    // Check if the JSON was valid
    if(job.isJsonValid())
    {
        // Check if the request was successful
        if(jobStatusCode == 200)
        {
            debug.success("Successful bakup job request");

            // Parse the response from Bakup to get the job list
            this->jobs = job.getVectoredResponse();

            // If debug mode is enabled
            if(!jobs.empty() && debug.getDebugMode())
            {
                // Print received jobs
                debug.info("Commands received:");
                for(const command_t &jobStruct: jobs)
                {
                    debug.info("Job to execute at " + to_string(jobStruct.targetExecutionTime));
                    for(const string &command: jobStruct.commands)
                    {
                        debug.info(command);
                    }
                }

                return true;
            }
            else // No jobs were found
            {
                debug.info("No commands were found in the job");
                return false;
            }
        }
        else // If it was not successful
        {
            // Hand each error output to the handle error function
            this->handleError(debug, job.getResponse(), job.getError());

            // If the maximum amount of retries has not been reached, try requesting job again
            if(retryCounter <= retryMaxCount)
            {
                // Print the error
                debug.error(
                        "Job request failed, will try again in " + to_string(this->getRetryTime()) + " seconds (Attempt " +
                        to_string(retryCounter) + " out of " + to_string(retryMaxCount) + ")");

                // Sleep until the job should be re-requested
                sleep(this->getRetryTime());

                // Try requesting the job again
                this->getJob(debug, ++retryCounter, retryMaxCount);
            }
            else
            {
                return false;
            }
        }
    }
}
bool Agent::handleError(Debug &debug, string httpResponse, cpr::Error error)
{
    debug.error("Sending job confirmation failed");

    // If the HTTP error is not empty
    if(httpResponse.length() != 0)
    {
        // Print the HTTP error response
        debug.error("Backup job request failed, printing error:");
        debug.error(httpResponse);
    }

    // If the error code is none zero
    if(static_cast<bool>(error))
    {
        // Get the libcurl error for printing
        string errorMessage = curl_easy_strerror(static_cast<CURLcode>(error.code));
        int errorCode = static_cast<CURLcode>(error.code);

        // Print the libcurl errors
        debug.error("libcurl error: " + to_string(errorCode) + ": " + errorMessage);
    }

    // Check if cpr error exists
    if(error.message.length() != 0)
    {
        // Print the CPR error
        debug.error("CPR error: " + error.message);
    }

    return true;
}

bool Agent::resetJob(Debug &debug)
{
    // Reset variables
    this->jobs = vector<command_t>();
    this->commandsOutput = "";

    // Print success and return
    debug.info("Reset temporary values in agent");
    return true;
}

int Agent::getNumberOfJobs() {
    return this->jobs.size();
}

bool Agent::processJobs(Debug &debug)
{
    // If the received job is a agent credential change, run synchronously
    if(this->jobs[0].refreshAgentCredentials)
    {
        Job newJob(debug, this->jobs[0], this->getBakupJobConfirmationURL(), this->getClientId(), this->getApiToken());
        this->refreshAgentCredentials(debug);
        this->skipNextPollTime = true;
    }
    else
    {
        // For each job in the jobs vector
        for(command_t job: this->jobs)
        {
            // Create a new thread with the job class constructor, passing in the job
            thread newJob([](Debug &debug, command_t &&job, string &&jobConfirmationURL, string &&clientId, string &&apiToken)
                          {
                              Job newJob(debug, job, jobConfirmationURL, clientId, apiToken);
                          },
                          ref(debug), job, this->getBakupJobConfirmationURL(), this->getClientId(), this->getApiToken());

            // Detach from the thread so that the main thread can continue running
            newJob.detach();
        }
    }

    return true;
}

void Agent::refreshAgentCredentials(Debug &debug)
{
    this->apiToken = this->readFile(this->apiTokenLocation);
    this->clientId = this->readFile(this->clientIdLocation);
    debug.success("Agent credentials successfully updated");
}
