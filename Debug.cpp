#include "Debug.h"

Debug::Debug(bool debugMode, Agent& agent) {
    // Set the given debug mode for the class
    this->debug = debugMode;
    this->version = agent.getAgentVersion();
}

void Debug::print(string debugString) {
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

bool Debug::getDebugMode() {
    return this->debug;
}
