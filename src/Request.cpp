#include <Request.h>

int Request::getBakupJob()
{
    // No parameters are required for this request, so create a blank variable
    cpr::Parameters parameters = cpr::Parameters{};

    // Add the authorisation token to the headers
    cpr::Header headers = cpr::Header{{"Authorization", this->authToken}};

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

Request::Request(string url, string authToken) : url(std::move(url)), authToken(std::move(authToken)) {}

vector<command_t> Request::parseBakupResponse(string &jsonString)
{
    // Create  the vector to return
    vector<command_t> commands;

    // Initiate a document to hold the json values from the response
    Document bakupResponse;

    // Parse the response
    bakupResponse.Parse(jsonString.c_str());

    // For each job command in the json object
    for (auto& job : bakupResponse.GetArray())
    {
        command_t temp;
        temp.targetExecutionTime = job["target_execution_time"].GetInt();

        for(auto& command : job["job_commands"].GetArray())
        {
            temp.commands.emplace_back(command.GetString());
        }
        // Add it to the returned vector
        commands.emplace_back(temp);
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
