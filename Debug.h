#ifndef BAKUP_AGENT_DEBUG_H
#define BAKUP_AGENT_DEBUG_H

#include <string>

using namespace std;

class Debug {
    private:
        // Is the program in debug mode?
        bool debug;

    public:
        // Constructor that takes the debug mode of the program
        explicit Debug(bool debugMode);

        // Print a string to console
        void print(string debugString);

        // Set debug mode
        void setDebugMode(bool newDebugMode);
};

#endif //BAKUP_AGENT_DEBUG_H
