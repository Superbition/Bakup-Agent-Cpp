#include "Response.h"

// Set the initial class variables
Response::Response(string url, string authToken) : url(std::move(url)), authToken(std::move(authToken)) {}

// Post data to a URL
int Response::apiPostData(cpr::Header &headers, string &postData, string &postResponse)
{
    // Make the post to Bakup
    cpr::Response r = cpr::Post(cpr::Url{this->url},
                                cpr::Header{headers},
                                cpr::Body{postData});

    // Set the returned content
    this->response = r.text;

    // return the status code
    return r.status_code;
}

// Send job confirmation information back to bakup
int Response::postJobConfirmation(string &postData, string &postResponse)
{
    // Add the authorisation token to the headers
    cpr::Header headers = cpr::Header{{"Authorization", this->authToken}, {"Content-Type", "text/json"}};

    // Variable to store response data inside
    string responseData;

    // Post the data
    int responseCode = this->apiPostData(headers, postData, responseData);

    // Set the response data that is returned from Bakup
    this->response = responseData;

    // Return the response code
    return responseCode;
}