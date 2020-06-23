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
#include "include/rapidjson/include/rapidjson/document.h"
#include "include/rapidjson/include/rapidjson/writer.h"
#include "include/rapidjson/include/rapidjson/stringbuffer.h"
#include <bits/stdc++.h>
#include "cpr/include/cpr/cpr.h"
#include "cpr/include/cpr/parameters.h"
#include "cpr/include/cpr/cprtypes.h"

std::string readFile(std::string &fileLocation);

void currentDateTime(char* dateTime);

int apiGetRequest(const std::string &url, cpr::Parameters &parameters, cpr::Header &headers, std::string &content);

int requestBakupUpdate(const std::string &url, const std::string &authorisationToken, std::string &content);

std::vector<std::string> parseBakupResponse(std::string &jsonString);

int processCommand(const char *command, std::string mainDirectory, std::string workingDirectory, std::string &output);

int main(int argc, char* argv[]);

#endif //BAKUP_AGENT_MAIN_H
