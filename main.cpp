#include "main.h"
using namespace std;

static void bakup_daemon()
{
//    pid_t pid;
//
//    /* Fork off the parent process */
//    pid = fork();
//
//    /* An error occurred */
//    if (pid < 0)
//        exit(EXIT_FAILURE);
//
//    /* Success: Let the parent terminate */
//    if (pid > 0)
//        exit(EXIT_SUCCESS);
//
//    /* On success: The child process becomes session leader */
//    if (setsid() < 0)
//        exit(EXIT_FAILURE);
//
//    /* Catch, ignore and handle signals */
//    //TODO: Implement a working signal handler */
//    signal(SIGCHLD, SIG_IGN);
//    signal(SIGHUP, SIG_IGN);
//
//    /* Fork off for the second time*/
//    pid = fork();
//
//    /* An error occurred */
//    if (pid < 0)
//        exit(EXIT_FAILURE);
//
//    /* Success: Let the parent terminate */
//    if (pid > 0)
//        exit(EXIT_SUCCESS);
//
//    /* Set new file permissions */
//    umask(0);
//
//    /* Change the working directory to the root directory */
//    /* or another appropriated directory */
//    chdir("/");
//
//    /* Close all open file descriptors */
//    int x;
//    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
//    {
//        close (x);
//    }
//
//    /* Open the log file */
//    openlog ("Bakup", LOG_PID, LOG_DAEMON);
}

string readFile(string fileLocation) {
    std::ifstream t(fileLocation);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

vector<databaseToBackup> getDatabases(string configFile) {
    vector<databaseToBackup> databaseVector;
    databaseToBackup fin = {"user", "pass", "name"};
    databaseVector.push_back(fin);
    return databaseVector;
}

int main(void)
{
    //bakup_daemon();
    //syslog(LOG_NOTICE, "Bakup daemon started.");

    // Get the database names and credentials
    vector<databaseToBackup> databasesToBackup = getDatabases("credentials.txt");

    for (int i = 0; i < databasesToBackup.size(); i++) {
        cout << databasesToBackup[i].username << endl;
        cout << databasesToBackup[i].password << endl;
        cout << databasesToBackup[i].databaseName << endl;
        //syslog(LOG_NOTICE, "%s", databasesToBackup[i].username);
        //syslog(LOG_NOTICE, "%s", databasesToBackup[i].password);
        //syslog(LOG_NOTICE, "%s", databasesToBackup[i].databaseName);
    }

    bool running = true;
    int counter = 0;
    cout << running << endl;
    while(running) {
        sleep(5);
        counter++;
        cout << "Iteration: " << counter << endl;
        if (counter > 3) {
            running = false;
        }
    }

    //syslog(LOG_NOTICE, "Bakup daemon terminated.");
    //closelog();

    return EXIT_SUCCESS;
}