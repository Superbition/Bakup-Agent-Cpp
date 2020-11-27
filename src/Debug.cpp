#include "Debug.h"

Debug::Debug(bool debugMode, string agentVersion)
{
    // Set the given debug mode for the class
    this->debug = debugMode;
    this->version = agentVersion;
}

void Debug::print(string debugString) 
{
    // If the program is running in debug mode
    if(this->debug)
    {
        // Print the given string
        cout << "[" << time(NULL) << "@Bakup" << this->version << "] " << debugString << endl;
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
