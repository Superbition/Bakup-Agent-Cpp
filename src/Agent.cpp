#include <Agent.h>

Agent::Agent()
{
    this->authToken = this->readFile(this->authorisationLocation);
    this->userID = this->readFile(this->userIDLocation);
};

Agent::Agent(const Agent &obj)
{
    // Set the temp values in the new agent object
    this->authToken = obj.authToken;
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

string Agent::getAuthToken()
{
    return this->authToken;
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

bool Agent::handleError(Debug &debug, string httpResponse, cpr::Error error)
{
    debug.print("Sending job confirmation failed");

    // If the HTTP error is not empty
    if(httpResponse.length() != 0)
    {
        // Print the HTTP error response
        debug.print("Backup job request failed, printing error:");
        debug.print(httpResponse);
    }

    // If the error code is none zero
    if(static_cast<bool>(error))
    {
        // Get the libcurl error for printing
        string errorMessage = curl_easy_strerror(static_cast<CURLcode>(error.code));
        int errorCode = static_cast<CURLcode>(error.code);

        // Print the libcurl errors
        debug.print("libcurl error: " + to_string(errorCode) + ": " + errorMessage);
    }

    // Check if cpr error exists
    if(error.message.length() != 0)
    {
        // Print the CPR error
        debug.print("CPR error: " + error.message);
    }

    return true;
}

bool Agent::getJob(Debug &debug, int retryCounter, int retryMaxCount)
{
    // Get a job from Bakup
    Request job(this->getBakupRequestURL(), this->getAuthToken());
    int jobStatusCode = job.getBakupJob();

    // Check if the request was successful
    if(jobStatusCode == 200)
    {
        debug.print("Successful bakup job request");

        // Parse the response from Bakup to get the job list
        this->jobs = job.getVectoredResponse();

        // If debug mode is enabled
        if(!jobs.empty() && debug.getDebugMode())
        {
            // Print received jobs
            debug.print("Commands received:");
            for(command_t jobStruct: jobs)
            {
                debug.print("Job to execute at " + to_string(jobStruct.targetExecutionTime));
                for(string command: jobStruct.commands)
                {
                    debug.print(command);
                }
            }

            return true;
        }
        else // No jobs were found
        {
            debug.print("No commands were found in the job");
            return false;
        }
    }
    else
    {
        this->handleError(debug, job.getResponse(), job.getError());
        if(retryCounter <= retryMaxCount)
        {
            debug.print("Job request failed, will try again in " + to_string(this->getRetryTime()) + " seconds (Attempt " + to_string(retryCounter) + " out of " + to_string(retryMaxCount) + ")");
            sleep(this->getRetryTime());
            this->getJob(debug, ++retryCounter, retryMaxCount);
        }
        else
        {
            return false;
        }
    }
}

bool Agent::runCommands(Debug &debug)
{
    // Store the exit status of the overall job
    bool exitStatus = true;

    // Check the job vector isn't empty
    if(!empty(this->jobs))
    {
        // if the desired execution time is in the future, sleep unitl then
        // Bakup should return jobs chronologically, so jobs won't execute late
        if(this->jobs[0].targetExecutionTime > time(NULL))
        {
            debug.print("Waiting " + to_string(this->jobs[0].targetExecutionTime - time(NULL)) + " seconds until desired execution time of command");
            sleep(this->jobs[0].targetExecutionTime - time(NULL));
        }

        // Create a string buffer and writer for creating a JSON string
        StringBuffer s;
        Writer<StringBuffer> writer(s);
        writer.StartObject();
        writer.Key("send_attempt");
        writer.Int(1);
        writer.Key("command_output");
        writer.StartArray();

        for(int i = 0; i < jobs[0].commands.size(); i++)
        {
            // Start a new object within the outer JSON object
            writer.StartObject();

            // Set up the command and working directory
            Command command(jobs[0].commands[i]);
            // Run the command and get the exit code
            int commandStatusCode = command.process();
            // Get the output of the command
            string result = command.getOutput();

            // Write the output and status of the command to the JSON object
            writer.Key("command");
            writer.String(jobs[0].commands[i].c_str());
            writer.Key("status_code");
            writer.Int(commandStatusCode);
            writer.Key("result");
            writer.String(result.c_str());
            writer.EndObject();

            // If the command didn't execute properly
            if(commandStatusCode != EXIT_SUCCESS)
            {
                exitStatus = false;
                break;
            }
        }

        // End the JSON string
        writer.EndArray();
        writer.EndObject();

        // Convert the JSON object to a string
        this->commandsOutput = s.GetString();
    }

    // remove the element from the vector
    this->jobs.erase(this->jobs.begin());
    return exitStatus;
}

bool Agent::reportResults(Debug &debug)
{
    // Build the response object to send command output back to Bakup
    Response response(this->getBakupJobConfirmationURL(), this->getAuthToken());

    // Execute and get the status
    int jobConfStatus = response.postJobConfirmation(this->commandsOutput);
    string jobConfOutput = response.getResponse();

    // If the job failed
    if(jobConfStatus != 200)
    {
        // Handle reporting the error to Bakup
        this->handleError(debug, jobConfOutput, response.getError());
        // Asynchronously retry sending the result to Bakup
        std::async(std::launch::async, &Agent::asyncReportResults, *this, ref(debug), 2, 5);
        return false;
    }
    else
    {
        debug.print("Successfully sent job confirmation");
        debug.print("Job confirmation response: " + to_string(jobConfStatus) + ": " + jobConfOutput);
    }

    return true;
}

bool Agent::asyncReportResults(Debug &debug, int counter, int maxRetry)
{
    // Check the maximum number of retries hasn't been reached
    if (counter <= maxRetry)
    {
        // Build the response object to send command output back to Bakup
        Response response(this->getBakupJobConfirmationURL(), this->getAuthToken());

        // Update the send attempt counter in the payload
        Document tempDoc;
        tempDoc.Parse(this->commandsOutput.c_str());
        tempDoc.FindMember("send_attempt")->value.SetInt(counter);
        StringBuffer buffer;
        buffer.Clear();
        Writer<StringBuffer> writer(buffer);
        tempDoc.Accept(writer);
        this->commandsOutput = buffer.GetString();

        // Execute and get the status
        int jobConfStatus = response.postJobConfirmation(this->commandsOutput);
        string jobConfOutput = response.getResponse();

        // If the job failed
        if (jobConfStatus != 200)
        {
            // Output status and wait
            debug.print("Job request failed, will try again in " + to_string(this->getRetryTime()) + " seconds (Attempt " + to_string(counter) + " out of " + to_string(maxRetry) + ")");
            sleep(this->getRetryTime());
            // Retry sending the result to Bakup
            std::async(&Agent::asyncReportResults, *this, ref(debug), ++counter, maxRetry);
            return false;
        }
        else
        {
            // Job was successful, print status and exit
            debug.print("Successfully sent job confirmation");
            debug.print("Job confirmation response: " + to_string(jobConfStatus) + ": " + jobConfOutput);
        }
    }

    return true;
}


bool Agent::resetJob(Debug &debug)
{
    // Reset variables
    this->jobs = vector<command_t>();
    this->commandsOutput = "";

    // Print success and return
    debug.print("Reset temporary values in agent");
    return true;
}

int Agent::getNumberOfJobs() {
    return this->jobs.size();
}
