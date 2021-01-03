#include <Response.h>

// Set the initial class variables
Response::Response(string url, string clientId, string authToken) : url(std::move(url)), clientId(std::move(clientId)), authToken(std::move(authToken)) {}

// Post data to a URL
int Response::apiPostData(cpr::Header &headers, string &postData, string &postResponse)
{
    // Make the post to Bakup
    cpr::Response r = cpr::Post(cpr::Url{this->url},
                                cpr::Header{headers},
                                cpr::Body{postData});

    // Set the returned content
    postResponse = r.text;

    // Get error class
    this->error = r.error;

    // return the status code
    return r.status_code;
}

// Send job confirmation information back to bakup
int Response::postJobConfirmation(string &postData)
{
    // Add the authorisation token to the headers
    cpr::Header headers = cpr::Header{{"ClientID", this->clientId}, {"Authorization", "Bearer " + this->authToken}, {"Content-Type", "text/json"}};

    // Variable to store response data inside
    string responseData;

    // Post the data
    int responseCode = this->apiPostData(headers, postData, responseData);

    // Set the response data that is returned from Bakup
    this->response = responseData;

    // Return the response code
    return responseCode;
}

// Send job confirmation information back to bakup
int Response::postJobError(string &postData)
{
    // Add the authorisation token to the headers
    cpr::Header headers = cpr::Header{{"ClientID", this->clientId}, {"Authorization", "Bearer " + this->authToken}, {"Content-Type", "text/json"}};

    // Variable to store response data inside
    string responseData;

    // Post the data
    int responseCode = this->apiPostData(headers, postData, responseData);

    // Set the response data that is returned from Bakup
    this->response = responseData;

    // Return the response code
    return responseCode;
}

string Response::getResponse()
{
    return this->response;
}

cpr::Error Response::getError()
{
    return this->error;
}

cpr::ErrorCode Response::getErrorCode()
{
    return this->error.code;
}

string Response::getErrorMessage()
{
    return this->error.message;
}
