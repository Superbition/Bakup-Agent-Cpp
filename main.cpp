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

// Send an API Get Request and return the JSON response
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

// Send a request for configuration updates
string requestBakupUpdate(string &url, string &authorisationToken)
{
    // No parameters are required for this request, so create a blank variable
    cpr::Parameters parameters = cpr::Parameters{};

    // Add the authorisation token to the headers
    cpr::Header headers = cpr::Header{{"Authorization", authorisationToken}};

    // Make the request to bakup
    string response = apiGetRequest(url, parameters, headers);

    // Return the response
    return response;
}

// The main function that handles the program loop
int main()
{
    // A bool to toggle running as daemon or not
    bool runAsDaemon = false;

    if (runAsDaemon)
    {
        bakup_daemon();
        syslog(LOG_NOTICE, "Bakup daemon started.");
    }

    // Store the location of the config file
    string authorisationLocation = "../AUTH_TOKEN";

    // Get the config file contents
    const string authToken = readFile(authorisationLocation);

    // Store the base URL
    const string baseUrl = "https://bakup.io";

    // Store the api version base url
    const string apiVersionBaseUrl = "/v";

    // Store the api version to be used in URLs
    const string apiVersion = "1";

    // Store the url to check for bakups
    const string bakupRequestUrl = "/bakup/request";

    if (runAsDaemon)
    {
        syslog(LOG_NOTICE, "Bakup daemon terminated.");
        closelog();
    }

    return EXIT_SUCCESS;
}
