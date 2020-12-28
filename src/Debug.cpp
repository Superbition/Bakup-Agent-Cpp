#include "Debug.h"

Debug::Debug(bool debugMode, string agentVersion)
{
    // Set the given debug mode for the class
    this->debug = debugMode;
    this->version = agentVersion;
}

Debug::Debug(const Debug &obj)
{
    // Set the debug values in the new debug object
    this->debug = obj.debug;
    this->version = obj.version;
}

string Debug::getDateTime()
{
    // Get the current time
    time_t rawTime;
    time(&rawTime);

    // Create the char array to hold the time string
    char timeString[100];

    // If the datetime is created successfully, return it
    if (strftime(timeString, sizeof(timeString), "%Y-%m-%dT%H:%M:%S", localtime(&rawTime)))
    {
        return string(timeString);
    }
    else // if not, return failed
    {
        return "FAILED TO GET TIME";
    }
}

void Debug::info(string infoString)
{
    // If the program is running in debug mode
    if(this->debug)
    {
        // Print the given string
        cout << "[" << this->getDateTime() << " Bakup " << this->version << "] [info] " << infoString << endl;
    }
}

void Debug::success(string successString)
{
    // If the program is running in debug mode
    if(this->debug)
    {
        // Print the given string
        cout << "[" << this->getDateTime() << " Bakup " << this->version << "] [" << this->colourGreen << "success" << this->colourReset << "] " << successString << endl;
    }
}

void Debug::error(string errorString)
{
    // If the program is running in debug mode
    if(this->debug)
    {
        // Print the given string
        cerr << "[" << this->getDateTime() << " Bakup " << this->version << "] [" << this->colourRed << "error" << this->colourReset << "] " << errorString << endl;
    }
}

void Debug::setDebugMode(bool newDebugMode)
{
    this->debug = newDebugMode;
}

bool Debug::getDebugMode() 
{
    return this->debug;
}
