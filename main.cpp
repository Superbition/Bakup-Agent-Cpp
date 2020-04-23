#include "main.h"

using namespace std;
using namespace rapidjson;

static void bakup_daemon()
{
    pid_t pid;

    // Fork off the parent process
    pid = fork();

    // An error occurred
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Success: Let the parent terminate
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    // On success: The child process becomes session leader
    if (setsid() < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Catch, ignore and handle signals
    //TODO: Implement a working signal handler
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // Fork off for the second time
    pid = fork();

    // An error occurred
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Success: Let the parent terminate
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    // Set new file permissions
    umask(0);

    // Change the working directory to the root directory or another appropriated directory
    // chdir("/");

    // Close all open file descriptors
    for (int x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }

    // Open the log file
    openlog ("Bakup", LOG_PID, LOG_DAEMON);
}

// Read a file and return a string
string readFile(string &fileLocation)
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
    fileText.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

    return fileText;
}

// A function to convert a given string to lowercase
string toLower(const string &text)
{
    // Convert the string to a c style string by first creating a char array with length of the string
    char * ctext = new char [text.length()+1];

    // Then copy the text contents in to the new ctext char array
    strcpy (ctext, text.c_str());

    // The locale that is to be used when converting a char in this string
    locale loc;

    // Go through each character of the string
    for (int i = 0; i < text.length(); i++)
    {
        ctext[i] = tolower(ctext[i], loc);
    }

    // Return the c string converted back to a string
    return string(ctext);
}

// Given a reference to a 20 byte char array, populate it with a datetime
void currentDateTime(char* dateTime)
{
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

// Send and API Get Request and return the JSON response
string apiGetRequest(const string &url, cpr::Parameters &parameters, cpr::Header &headers)
{
    // Make the request to Bakup
    auto r = cpr::Get(cpr::Url{url},
             parameters,
             cpr::Header{headers});

    // If the request was successful
    if (r.status_code == 200)
    {
        return r.text;
    }
    else // Else the request was not successful
    {
        return "";
    }
}

// Load config will parse the config json file
void loadConfigFile(const char* &configContents, map<string, string> &bakupCredentials, map<string, string> &databaseCredentials, map<string, string> &locationCredentials)
{
    // Initiate a document to hold the json values from the config file
    Document document;

    // Parse the config file
    document.Parse(configContents);

    // For each member inside bakup_credentials, add them to the map
    for (auto& member : document["bakup_credentials"].GetObject())
    {
        bakupCredentials.insert(std::pair<string, string>(member.name.GetString(), member.value.GetString()));
    }

    // For each member inside database_credentials, add them to the map
    for (auto& member : document["database_credentials"].GetObject())
    {
        databaseCredentials.insert(std::pair<string, string>(member.name.GetString(), member.value.GetString()));
    }

    // For each member inside bakup_locations, add them to the map
    for (auto& member : document["backup_locations"].GetObject())
    {
        locationCredentials.insert(std::pair<string, string>(member.name.GetString(), member.value.GetString()));
    }
}

// The main function that handles the program loop
int main()
{
    // A bool to toggle running as daemon or not
    bool runAsDaemon = false;

    // Store the Bakup credentials to be used in requests to Bakup
    map<string, string> bakupCredentials;

    // Store the credentials of local databases
    map<string, string> databaseCredentials;

    // Store the credentials of the bakup locations
    map<string, string> locationCredentials;

    if (runAsDaemon)
    {
        bakup_daemon();
        syslog(LOG_NOTICE, "Bakup daemon started.");
    }

    // Store the location of the config file
    string configLocation = "../config.json";

    // Get the config file contents
    string configContents = readFile(configLocation);

    // Convert to char* for parsing
    const char* configString = configContents.c_str();

    // Parse the values of the json values and load them in to memory
    loadConfigFile(configString, bakupCredentials, databaseCredentials, locationCredentials);

    // Test request
    const string url = "http://a8abfe30-58bf-4d3b-aa2f-780002e0e48d.mock.pstmn.io/v1/config/request";
    cpr::Parameters parameters = cpr::Parameters{{"test", "parameter"}};
    cpr::Header headers = cpr::Header{{"Authorization", bakupCredentials["api_key"]}};
    cout << apiGetRequest(url, parameters, headers) << endl;

    if (runAsDaemon)
    {
        syslog(LOG_NOTICE, "Bakup daemon terminated.");
        closelog();
    }

    return EXIT_SUCCESS;
}
