#ifndef BAKUP_AGENT_DEBUG_H
#define BAKUP_AGENT_DEBUG_H

#include "Agent.h"
#include <string>
#include <iostream>
#include <ctime>

using namespace std;

class Debug 
{
    private:
        // Is the program in debug mode?
        bool debug;

        // Get the agent version
        string version;

    public:
        // Constructor that takes the debug mode of the program
        explicit Debug(bool debugMode, Agent &agent);

        // Print a string to console
        void print(string debugString);

        // Set debug mode
        void setDebugMode(bool newDebugMode);

        // Get debug mode
        bool getDebugMode();
};

#endif //BAKUP_AGENT_DEBUG_H
