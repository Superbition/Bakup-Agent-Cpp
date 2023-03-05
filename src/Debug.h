#ifndef BAKUP_AGENT_DEBUG_H
#define BAKUP_AGENT_DEBUG_H

#include <string>
#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

class Debug 
{
    private:
        // Is the program in debug mode?
        bool debug;

        // Get the agent version
        string version;

        // Hold colour variables
        const string colourGreen = "\033[;32m";
        const string colourRed = "\033[;31m";
        const string colourReset = "\033[0m";

    public:
        // Constructor that takes the debug mode of the program
        Debug(bool debugMode, string agentVersion);

        // Copy constructor
        Debug(const Debug &obj);

        // Generate datetime string
        string getDateTime();

        // Print a string to console
        void info(string infoString, bool overrideDebug = false);

        // Print an array to console
        void info(vector<string> infoArray, bool newLine = false, bool overrideDebug = false);

        // Print a success string to console
        void success(string successString, bool overrideDebug = true);

        // Print an error string to console
        void error(string errorString, bool overrideDebug = true);

        // Set debug mode
        void setDebugMode(bool newDebugMode);

        // Get debug mode
        bool getDebugMode();
};

#endif //BAKUP_AGENT_DEBUG_H
