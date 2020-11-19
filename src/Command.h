#ifndef BAKUP_AGENT_COMMAND_H
#define BAKUP_AGENT_COMMAND_H

#include <string>
#include <array>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <utility>

using namespace std;

class Command
{
    private:
        // The command being processed
        string command;

        // Program output
        string output;

    public:
        // Constructor
        Command(string &command);

        // Process a command
        int process();

        // Get the output from the command
        string getOutput();
};

#endif //BAKUP_AGENT_COMMAND_H
