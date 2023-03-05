#include "Debug.h"

Debug::Debug(bool debugMode, string agentVersion)
{
    // Set the given debug mode for the class
    this->debug = debugMode;
    this->version = agentVersion;
}

Debug::Debug(const Debug &obj)
{
    // Set the debug values in the new debug object
    this->debug = obj.debug;
    this->version = obj.version;
}

string Debug::getDateTime()
{
    // Get the current time
    time_t rawTime;
    time(&rawTime);

    // Create the char array to hold the time string
    char timeString[100];

    // If the datetime is created successfully, return it
    if(strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localtime(&rawTime)))
    {
        return string(timeString);
    }
    else // if not, return failed
    {
        return "FAILED TO GET TIME";
    }
}

void Debug::info(string infoString, bool overrideDebug)
{
    // If the program is running in debug mode
    if(this->debug || overrideDebug)
    {
        // Print the given string
        cout << "[" << this->getDateTime() << " Bakup " << this->version << "] [info] " << infoString << endl;
    }
}

void Debug::info(vector<string> infoArray, bool newLine, bool overrideDebug)
{
    // If the program is running in debug mode
    if(this->debug || overrideDebug)
    {
        if(newLine)
        {
            string dateTime = this->getDateTime();
            for(int i = 0; i < infoArray.size(); i++)
            {
                cout << "[" << dateTime << " Bakup " << this->version << "] [info] " << infoArray[i] << endl;
            }
        }
        else
        {
            string toPrint = "";

            for(const string &info: infoArray)
            {
                toPrint.append(info + ", ");
            }

            cout << "[" << this->getDateTime() << " Bakup " << this->version << "] [info] " << toPrint.substr(0, toPrint.size()-2) << endl;
        }
    }
}

void Debug::success(string successString, bool overrideDebug)
{
    // If the program is running in debug mode
    if(this->debug || overrideDebug)
    {
        // Print the given string
        cout << "[" << this->getDateTime() << " Bakup " << this->version << "] [" << this->colourGreen << "success" << this->colourReset << "] " << successString << endl;
    }
}

void Debug::error(string errorString, bool overrideDebug)
{
    // If the program is running in debug mode
    if(this->debug || overrideDebug)
    {
        // Print the given string
        cerr << "[" << this->getDateTime() << " Bakup " << this->version << "] [" << this->colourRed << "error" << this->colourReset << "] " << errorString << endl;
    }
}

void Debug::setDebugMode(bool newDebugMode)
{
    this->debug = newDebugMode;
}

bool Debug::getDebugMode() 
{
    return this->debug;
}
