#include <iostream>
#include "Debug.h"

Debug::Debug(bool debugMode) {
    // Set the given debug mode for the class
    this->debug = debugMode;
}

void Debug::Print(string debugString) {
    // If the program is running in debug mode
    if(this->debug)
    {
        // Print the given string
        cout << debugString << endl;
    }
}

void Debug::setDebugMode(bool newDebugMode)
{
    this->debug = newDebugMode;
}


