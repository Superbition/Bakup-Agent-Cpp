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
#include <string>
#include <vector>

struct databaseToBackup {
    const char *username;
    const char *password;
    const char *databaseName;
};

static void bakup_daemon();

int main();

#endif //BAKUP_AGENT_MAIN_H
