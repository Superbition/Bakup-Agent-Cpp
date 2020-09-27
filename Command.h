#ifndef BAKUP_AGENT_COMMAND_H
#define BAKUP_AGENT_COMMAND_H

#include <string>
#include <array>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <utility>

using namespace std;

class Command {

    // The command being processed
    string command;

    // The main directory the program is working in
    string mainDirectory;

    // The working directory the temp files will be made in
    string workingDirectory;

    // Program output
    string output;

public:
    // Constructor
    Command(string &command, string &workingDirectory);

    // Process a command
    int process();

    // Get the output from the command
    string getOutput();

};

#endif //BAKUP_AGENT_COMMAND_H
