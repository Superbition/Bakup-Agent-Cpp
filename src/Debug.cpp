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
    time_t rawTime;
    time(&rawTime);
    struct tm* fullTime = gmtime(&rawTime);

    return to_string(1900 + fullTime->tm_year) + "-"
            + to_string(fullTime->tm_mon) + "-"
            + to_string(fullTime->tm_mday) + "T"
            + to_string(fullTime->tm_hour) + ":"
            + to_string(fullTime->tm_min) + ":"
            + to_string(fullTime->tm_sec);
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
