#ifndef BAKUP_AGENT_MAIN_H
#define BAKUP_AGENT_MAIN_H

#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <csignal>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <cstring>
#include <vector>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <exception>
#include <ctime>

std::string readFile(std::string);

struct databaseToBackup {
    std::basic_string<char> databaseName;
    std::basic_string<char> username;
    std::basic_string<char> password;
    std::basic_string<char> engine;
};

static void bakup_daemon();

std::string toLower(const std::string &text);

std::vector<databaseToBackup> getDatabases(std::string configFileContents);

void currentDateTime(char* dateTime);

int main();

#endif //BAKUP_AGENT_MAIN_H
