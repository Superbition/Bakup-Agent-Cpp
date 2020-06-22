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
int apiGetRequest(const string &url, cpr::Parameters &parameters, cpr::Header &headers, string &content)
{
    // Make the request to Bakup
    auto r = cpr::Get(cpr::Url{url},
             parameters,
             cpr::Header{headers});

    // Set the returned content
    content = r.text;

    // return the status code
    return r.status_code;
}

// Send a request for configuration updates
int requestBakupUpdate(const string &url, const string &authorisationToken, string &content)
{
    // No parameters are required for this request, so create a blank variable
    cpr::Parameters parameters = cpr::Parameters{};

    // Add the authorisation token to the headers
    cpr::Header headers = cpr::Header{{"Authorization", authorisationToken}};

    // Variable to store content inside
    string http_content;

    // Make the request to bakup
    int responseCode = apiGetRequest(url, parameters, headers, http_content);

    // Set the content that is returned from the api get request function
    content = http_content;

    // Return the response
    return responseCode;
}

// Parse the bakup response json
vector<string> parseBakupResponse(string &jsonString)
{
    // Create  the vector to return
    vector<string> commands;

    // Initiate a document to hold the json values from the response
    Document bakupResponse;

    // Parse the response
    bakupResponse.Parse(jsonString.c_str());

    // For each job command in the json object
    for (auto& command : bakupResponse["job_commands"].GetArray())
    {
        // Add it to the returned vector
        commands.emplace_back(command.GetString());
    }

    // Return the values
    return commands;
}

// Process the given command and write the output to output
int processCommand(const char *command, string mainDirectory, string workingDirectory, string &output)
{
    // Switch to the temporary working directory
    chdir(workingDirectory.c_str());

    // An array to hold the output of the stream from the command process
    array<char, 128> buffer{};

    // Open a stream in read mode using the supplied command
    auto pipe = popen(command, "r");

    // If the stream fails to open, return an error
    if(!pipe)
    {
        output = "Popen failed to open a stream";
        return -1;
    }
    else // Otherwise, the stream can be used successfully
    {
        // Read from stream until EOF is found
        while(!feof(pipe))
        {
            // If the read data is not null
            if(fgets(buffer.data(), 128, pipe) != nullptr)
            {
                // Write the line of output to the output string
                output += buffer.data();
            }
        }
    }

    // Close the pipe and read the status code
    auto statusCode = pclose(pipe);

    // Switch back to the main directory
    chdir(mainDirectory.c_str());

    // Return the status code of the command
    return statusCode;
}

int apiPostData(const string &url, cpr::Header &headers, string &postData, string &response)
{
    // Make the post to Bakup
    cpr::Response r = cpr::Post(cpr::Url{url},
                      cpr::Header{headers},
                      cpr::Body{postData});

    // Set the returned content
    response = r.text;

    // return the status code
    return r.status_code;
}

int postJobConfirmation(const string &url, const string &authorisationToken, string &postData, string &response)
{
    // Add the authorisation token to the headers
    cpr::Header headers = cpr::Header{{"Authorization", authorisationToken}, {"Content-Type", "text/json"}};

    // Variable to store response data inside
    string responseData;

    // Post the data
    int responseCode = apiPostData(url, headers, postData, responseData);

    // Set the response data that is returned from Bakup
    response = responseData;

    // return response code
    return responseCode;
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

    // Test Bakup request
    string url = baseUrl + apiVersionBaseUrl + apiVersion + bakupRequestUrl;
    string bakupContent;
    int statusCode = requestBakupUpdate(url, authToken, bakupContent);

    if (statusCode >= 400)
    {
        cout << "Error code " << statusCode << endl;
        cout << bakupContent << endl;
    }
    else
    {
        cout << "Successful request " << endl;
        cout << bakupContent << endl;
    }

    cout << "Parsing bakup response" << endl;

    string jobString = "{\"job_commands\":[\"mysqldump -ureadonly -ppassword mysql > bakupfile\",\"gzip bakupfile\",\"scp -qB bakupfile.gz pi:/home/ubuntu/bakups/bakupfile.gz\",\"rm bakupfile.gz\"]}";
    //string jobString = "{\"job_commands\":[\"ls\", \"pwd\", \"whoami\", \"ls /root\"]}";

    // Convert the JSON string in to a vector for looping through
    vector<string> jobs = parseBakupResponse(jobString);

    // Get the current working directory
    string cwd = filesystem::current_path();

    char timestamp[20];
    // Generate a name for a temp directory to work in
    currentDateTime(timestamp);
    string workingDir = string("temp") + timestamp;
    string absoluteWorkingDir = cwd + "/" + workingDir;
    mkdir(absoluteWorkingDir.c_str(), S_IRWXU);

    // Create a string buffer and writer for creating a JSON string
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartArray();

    for (int i = 0; i < jobs.size(); i++)
    {
        writer.StartObject();
        // The command needs to have stderr redirected to stdout so that both can be captured
        string command = jobs[i] + " 2>&1";

        // A string to store the result in
        string result;

        // Run the command and get the exit code
        int commandStatusCode = processCommand(command.c_str(), cwd, absoluteWorkingDir, result);

        writer.Key("command");
        writer.String(jobs[i].c_str());
        writer.Key("status_code");
        writer.Int(commandStatusCode);
        writer.Key("result");
        writer.String(result.c_str());
        writer.EndObject();

        // If the command didn't execute properly
        if(commandStatusCode != EXIT_SUCCESS)
        {
            break;
        }
    }

    // End the JSON string
    writer.EndArray();

    string jobStatusString = s.GetString();
    cout << jobStatusString << endl;
    const string jobConfirmationUrl = "localhost/api/v1/bakup/confirm";
    string jobResponse;
    int jobConfStatus = postJobConfirmation(jobConfirmationUrl, authToken, jobStatusString, jobResponse);

    cout << jobConfStatus << endl;
    cout << jobResponse << endl;

    // Remove the temporary directory
    rmdir(absoluteWorkingDir.c_str());

    if (runAsDaemon)
    {
        syslog(LOG_NOTICE, "Bakup daemon terminated.");
        closelog();
    }

    return EXIT_SUCCESS;
}
