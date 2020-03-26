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

// Read a file and return a string
string readFile(string fileLocation)
{
    // Open the file stream using the given file location
    std::ifstream fileStream(fileLocation);

    // The place holder string to hold the contents of the file
    std::string fileText;

    // Seek to the end of the file
    fileStream.seekg(0, std::ios::end);

    // Reserve memory for the string using the new position in the file stream
    fileText.reserve(fileStream.tellg());

    // Move the position back to the start of the file
    fileStream.seekg(0, std::ios::beg);

    // Assign the file text using the iterator from the file stream
    fileText.assign((std::istreambuf_iterator<char>(fileStream)),std::istreambuf_iterator<char>());

    return fileText;
}

// Parse the database name, credentials and engine type from the configuration file
vector<databaseToBackup> getDatabases(string configFileContents)
{
    // A vector of database credentials
    vector<databaseToBackup> databaseVector;

    // A temporary string to hold each new line from the config
    string tmp;

    // A temporary databaseToBackup struct to hold information whilst the file is fully read
    databaseToBackup tmpStruct;

    // Convert the original string in to a stream for processing
    stringstream ss(configFileContents);

    // A vector to hold each line of the config file
    vector<string> configLines;

    // For every line in the config file
    while (ss >> tmp)
    {
        // If the line is not blank
        if (!tmp.empty())
        {
            // If the line is a databaseName
            const char first = tmp[0];
            const char last = tmp[tmp.size() - 1];
            if (first == '[' && last == ']')
            {
                // Set the database name
                tmpStruct.databaseName = tmp.substr(1, tmp.size() - 2);
            }
            else
            {
                // Else this line is a value, set the delimiter
                string delimiter = "=";

                // Get the token from the line by delimiting
                string token = tmp.substr(0, tmp.find(delimiter));

                // Get the value using the same method
                string value = tmp.substr(tmp.find(delimiter) + 1, tmp.size() - 1);

                // Compare the token value to the struct values
                if (token == "username")
                {
                    // If the token is username, add the value to the struct
                    if (tmpStruct.username.empty() && !tmpStruct.databaseName.empty())
                    {
                        // if the username has not been set, set it
                        tmpStruct.username = value;
                    }
                }
                else if (token == "password")
                {
                    // If the token is password, add the value to the struct
                    if (tmpStruct.password.empty() && !tmpStruct.databaseName.empty())
                    {
                        // if the password has not been set, set it
                        tmpStruct.password = value;
                    }
                }
                else if (token ==  "engine")
                {
                    // If the token is engine, add the value to the struct
                    if (tmpStruct.engine.empty() && !tmpStruct.databaseName.empty())
                    {
                        // if the engine has not been set, set it
                        tmpStruct.engine = value;
                    }
                }

                // If the temporary struct has been completed
                if (!tmpStruct.databaseName.empty() && !tmpStruct.username.empty() && !tmpStruct.password.empty() && !tmpStruct.engine.empty())
                {
                    // Add the struct to the main database vector
                    databaseVector.push_back(tmpStruct);

                    // Reset the tmpStruct to store database details
                    tmpStruct = {};
                }
            }
        }
    }

    return databaseVector;
}

// Given a reference to a 20 byte char array, populate it with a datetime
void currentDateTime(char* dateTime) {
    // Setup the specialised time variable for holding the current datetime
    time_t rawTime;

    // Struct to hold localised raw time codes
    struct tm * timeInfo;

    // The buffer to store the formatted datetime value in, should not be more that 20 bytes
    char buffer [20];

    // Populate datetime and struct with localised time
    time (&rawTime);
    timeInfo = localtime (&rawTime);

    // Format the raw time in to the given format string
    strftime(buffer, 20, "%Y-%m-%dT%H:%M:%S", timeInfo);

    // Copy the contents of the buffer in to the dateTime parameter
    strncpy(dateTime, buffer, 20);
}

// The main function that handles the program loop
int main(void)
{
    //bakup_daemon();
    //syslog(LOG_NOTICE, "Bakup daemon started.");

    // Get the database names and credentials
    vector<databaseToBackup> databasesToBackup = getDatabases(readFile("../credentials.txt"));

    char dateTime [20];
    currentDateTime(dateTime);
    cout << dateTime << "\n";

    for (int i = 0; i < databasesToBackup.size(); i++)
    {
        string command = "mysqldump --single-transaction --routines --triggers --user=" + databasesToBackup[i].username + " --password=" + databasesToBackup[i].password + " " + databasesToBackup[i].databaseName + " --result-file=\"" + databasesToBackup[i].databaseName + dateTime + ".bakup\"";
        system(command.c_str());
    }

    bool running = true;
    int counter = 0;
    while(running) {
        sleep(1);
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