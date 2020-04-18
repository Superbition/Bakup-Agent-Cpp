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
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
#include <bits/stdc++.h>
#include "include/cpr/cpr.h"
#include "include/cpr/parameters.h"

std::string readFile(std::string &fileLocation);

static void bakup_daemon();

std::string toLower(const std::string &text);

void currentDateTime(char* dateTime);

void loadConfigFile(const char* &configContents, std::map<std::string, std::string> &bakupCredentials, std::map<std::string, std::string> &databaseCredentials, std::map<std::string, std::string> &locationCredentials);

int main();

#endif //BAKUP_AGENT_MAIN_H
