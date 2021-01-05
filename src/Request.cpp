#include <Request.h>

int Request::getBakupJob()
{
    // No parameters are required for this request, so create a blank variable
    cpr::Parameters parameters = cpr::Parameters{};

    // Add the authorisation token to the headers
    cpr::Header headers = cpr::Header{{"ClientID", this->clientId}, {"Authorization", "Bearer " + this->apiToken}};

    // Variable to store content inside
    string http_content;

    // Make the request to bakup
    int responseCode = this->apiGetRequest(parameters, headers, http_content);

    // Set the content that is returned from the api get request function
    this->response = http_content;

    // Check the response code to see if the request was successful
    if(responseCode == 200)
    {
        // If so, parse the response
        this->vectorResponse = parseBakupResponse(this->response);
    }

    // Return the response
    return responseCode;
}

int Request::apiGetRequest(cpr::Parameters &parameters, cpr::Header &headers, string &content)
{
    // Make the request to Bakup
    auto r = cpr::Get(cpr::Url{this->url},
                        parameters,
                        headers);

    // Set the returned content
    content = r.text;

    // Get error class
    this->error = r.error;

    // return the status code
    return r.status_code;
}

Request::Request(string url, string clientId, string apiToken, Debug &debug)
        : url(std::move(url)), clientId(std::move(clientId)), apiToken(std::move(apiToken)), debug(ref(debug)) {}

vector<command_t> Request::parseBakupResponse(string &jsonString)
{
    // Create  the vector to return
    vector<command_t> commands;

    // Initiate a document to hold the json values from the response
    Document bakupResponse;

    // Check if the JSON is valid
    if(!bakupResponse.Parse(jsonString.c_str()).HasParseError())
    {
        // Parse the response
        bakupResponse.Parse(jsonString.c_str());

        // For each job command in the json object
        for (auto& job : bakupResponse.GetArray())
        {
            command_t temp;
            temp.id = job["id"].GetString();

            // Check for a target execution time
            if(job.HasMember("target_execution_time"))
            {
                temp.targetExecutionTime = job["target_execution_time"].GetInt();
            }
            else // If there is no specified execution time, execute now
            {
                temp.targetExecutionTime = 0;
            }

            // Get the jobs
            for(auto& command : job["job_commands"].GetArray())
            {
                temp.commands.emplace_back(command.GetString());
            }

            // Check for refresh agent credentials setting
            if(job.HasMember("refresh_agent_credentials"))
            {
                temp.refreshAgentCredentials = job["refresh_agent_credentials"].GetBool();
            }


            // Add it to the returned vector
            commands.emplace_back(temp);
        }
    }
    else
    {
        this->debug.error("Invalid JSON received from Bakup");
        this->debug.info(jsonString);
        this->JsonValid = false;
    }

    // Return the values
    return commands;
}

string Request::getResponse()
{
    return this->response;
}

vector<command_t> Request::getVectoredResponse()
{
    return this->vectorResponse;
}

cpr::Error Request::getError()
{
    return this->error;
}

string Request::getErrorMessage()
{
    return this->error.message;
}

cpr::ErrorCode Request::getErrorCode()
{
    return this->error.code;
}

bool Request::isJsonValid() {
    return this->JsonValid;
}
