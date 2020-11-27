#include <Agent.h>

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

int Agent::getWaitTime()
{
    return this->pollTime;
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
    if (static_cast<bool>(error))
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

bool Agent::getJob(Debug &debug) {
    // Get a job from Bakup
    Request job(this->getBakupRequestURL(), this->getAuthToken());
    int jobStatusCode = job.getBakupJob();

    // A vector to store the output of the job request if successful
    vector<string> jobs;

    // Check if the request was successful
    if(jobStatusCode == 200)
    {
        debug.print("Successful bakup job request");

        // Parse the response from Bakup to get the job list
        jobs = job.getVectoredResponse();

        // If debug mode is enabled
        if(!jobs.empty())
        {
            // If there are jobs
            if(debug.getDebugMode())
            {
                // Print received jobs
                debug.print("Commands received:");
                for(string& command: jobs)
                {
                    debug.print(command);
                }
            }

            // Set the commands in the agent class
            this->commands = jobs;
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
        return false;
    }

}

bool Agent::runCommands(Debug &debug)
{
    // Store the exit status of the overall job
    bool exitStatus = true;

    // Check the job string isn't empty
    if(!commands.empty())
    {
        // Create a string buffer and writer for creating a JSON string
        StringBuffer s;
        Writer<StringBuffer> writer(s);
        writer.StartArray();

        for(int i = 0; i < commands.size(); i++)
        {
            // Start a new object within the outer JSON object
            writer.StartObject();

            // Set up the command and working directory
            Command command(commands[i]);
            // Run the command and get the exit code
            int commandStatusCode = command.process();
            // Get the output of the command
            string result = command.getOutput();

            // Write the output and status of the command to the JSON object
            writer.Key("command");
            writer.String(commands[i].c_str());
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

        // Convert the JSON object to a string
        this->commandsOutput = s.GetString();
    }

    return exitStatus;
}

bool Agent::reportResults(Debug &debug) {
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
    }
    else
    {
        debug.print("Successfully sent job confirmation");
        debug.print("Job confirmation response: " + to_string(jobConfStatus) + ": " + jobConfOutput);
    }

    return true;
}

bool Agent::resetJob(Debug &debug) {
    this->commands = vector<string>();
    this->commandsOutput = "";
    debug.print("Reset temporary values in agent");
    return true;
}
