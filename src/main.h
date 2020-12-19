#ifndef BAKUP_AGENT_MAIN_H
#define BAKUP_AGENT_MAIN_H

#include <Debug.h>
#include <Agent.h>
#include <Request.h>
#include <Command.h>
#include <Response.h>

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
#include <cpr/cpr.h>
#include <curl/curl.h>

int main(int argc, char* argv[]);

#endif //BAKUP_AGENT_MAIN_H
