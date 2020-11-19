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

int main(int argc, char* argv[]);

#endif //BAKUP_AGENT_MAIN_H
