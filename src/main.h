#ifndef BAKUP_AGENT_MAIN_H
#define BAKUP_AGENT_MAIN_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
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
#include "../include/rapidjson/include/rapidjson/document.h"
#include "../include/rapidjson/include/rapidjson/writer.h"
#include "../include/rapidjson/include/rapidjson/stringbuffer.h"
#include <cpr/cpr.h>
//#include "../include/cpr/include/cpr/cpr.h"
//#include "../include/cpr/include/cpr/parameters.h"
//#include "../include/cpr/include/cpr/cprtypes.h"

void currentDateTime(char* dateTime);

int apiPostData(const std::string &url, cpr::Header &headers, std::string &postData, std::string &response);

int postJobConfirmation(const std::string &url, const std::string &authorisationToken, std::string &postData, std::string &response);

int main(int argc, char* argv[]);

#endif //BAKUP_AGENT_MAIN_H
