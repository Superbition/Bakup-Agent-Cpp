#ifndef BAKUP_AGENT_DEBUG_H
#define BAKUP_AGENT_DEBUG_H

#include <string>

using namespace std;

class Debug {
    // Is the program in debug mode?
    bool debug;

public:
    // Constructor that takes the debug mode of the program
    explicit Debug(bool debugMode);

    // Print a string to console
    void Print(string debugString);
};


#endif //BAKUP_AGENT_DEBUG_H
