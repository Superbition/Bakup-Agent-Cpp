#include "Agent.h"
#include <fstream>

std::string Agent::readFile(const std::string &fileLocation)
{
    // Open the file stream using the given file location
    std::ifstream fileStream(fileLocation);

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

    return fileText;
}

string Agent::getBakupRequestURL()
{
    return this->host + this->baseUrl + this->apiVersionBaseUrl + this->apiVersion + this->bakupRequestUrl;
}

string Agent::getAuthToken()
{
    return this->authToken;
}

string Agent::getBakupJobConfirmationURL()
{
    return this->host + this->baseUrl + this->apiVersionBaseUrl + this->apiVersion + this->bakupJobConfirmationUrl;
}

string Agent::getWorkingDirectory()
{
    return this->workingDirectory;
}

int Agent::getWaitTime() {
    return this->waitTime;
}
