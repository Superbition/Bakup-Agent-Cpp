#include "ResponseBuilder.h"

ResponseBuilder::ResponseBuilder()
{
    // Start the json object
    this->writer->StartObject();
}

string ResponseBuilder::build()
{
    // End the object
    this->writer->EndObject();

    // Return the string of the json object
    return std::string(this->json.GetString());
}

void ResponseBuilder::addErrorCode(int errorCode)
{
    this->writer->Key("error_code");
    this->writer->Int(errorCode);
}

void ResponseBuilder::addErrorMessage(string &errorMessage)
{
    this->writer->Key("error_message");
    this->writer->String(errorMessage.c_str());
}

void ResponseBuilder::addSendAttempt(int sendAttempt)
{
    this->writer->Key("send_attempt");
    this->writer->Int(sendAttempt);
}

void ResponseBuilder::addCommandOutputs(vector<commandOutput> &commandsOutput)
{
    // Start the array to hold the command's results
    this->writer->Key("command_output");
    this->writer->StartArray();

    // Loop through the vector of command outputs
    for(commandOutput &commandOutput: commandsOutput)
    {
        // Start an object
        this->writer->StartObject();

        // Add the command that was ran
        this->writer->Key("command");
        this->writer->String(commandOutput.command.c_str());

        // Add the status code
        this->writer->Key("status_code");
        this->writer->Int(commandOutput.statusCode);

        // Add the result
        this->writer->Key("result");
        this->writer->String(commandOutput.result.c_str());

        // End the object
        this->writer->EndObject();
    }

    // End the array
    this->writer->EndArray();
}

