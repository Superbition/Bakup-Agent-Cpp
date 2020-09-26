#include "Command.h"


int Command::process()
{
    // Redirect stderr to std out
    this->command = this->command + " 2>&1";

    // Switch to the temporary working directory
    chdir(workingDirectory.c_str());

    // An array to hold the output of the stream from the command process
    array<char, 128> buffer{};

    // Open a stream in read mode using the supplied command
    auto pipe = popen(command.c_str(), "r");

    // If the stream fails to open, return an error
    if(!pipe)
    {
        this->output = "Popen failed to open a stream";
        return -1;
    }
    else // Otherwise, the stream can be used successfully
    {
        // Read from stream until EOF is found
        while(!feof(pipe))
        {
            // If the read data is not null
            if(fgets(buffer.data(), 128, pipe) != nullptr)
            {
                // Write the line of output to the output string
                this->output += buffer.data();
            }
        }
    }

    // Close the pipe and read the status code
    auto statusCode = pclose(pipe);

    // Switch back to the main directory
    chdir(mainDirectory.c_str());

    // Return the status code of the command
    return statusCode;
}

Command::Command(string &command, string &workingDirectory)
{
    this->command = command;
    this->mainDirectory = get_current_dir_name();
    this->workingDirectory = workingDirectory;
}

string Command::getOutput()
{
    return this->output;
}