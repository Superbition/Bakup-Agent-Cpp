#include <main.h>

using namespace std;
using namespace rapidjson;

// The main function that handles the program loop
int main(int argc, char *argv[])
{
    // Initialise the agent class
    Agent agent;

    // Set the program to non-debug mode
    Debug debug(false, agent.getAgentVersion());

    // If there is a command line argument
    if(argc == 2)
    {
        // If the -d flag is passed
        if(strcmp(argv[1], "-d") == 0)
        {
            // Set the debug class to true, to log to command line
            debug.setDebugMode(true);
        }
    }
        // If there is more than one command line argument
    else if(argc > 2)
    {
        debug.error("Too many arguments passed");
        return EXIT_FAILURE;
    }

    // Check if this is the first time the agent is being ran, if so we need to send an initialisation ping
    if(!agent.checkFirstRunAndPing(debug))
    {
        // Exit, it failed
        return 1;
    }

    // Change user identity to given user ID
    int uid = stoi(agent.getUserID());
    int result = seteuid(uid);
    if(result < 0)
    {
        debug.error("Error switching from root to user ID: " + to_string(uid));
        return EXIT_FAILURE;
    }
    debug.success("Assuming user ID " + to_string(uid), false);

    // Get the program loop wait time
    int waitTime = agent.getWaitTime();

    // Main program loop
    while(true)
    {
        // Get a job from Bakup, second arg = attempt number, third arg = max attempts
        if(agent.getJob(debug, 1, 5))
        {
            // If there are jobs, process them asynchronously
            agent.processJobs(debug);
        }

        // Check if the loop should wait before executing
        if(agent.skipNextPollTime)
        {
            // If so, don't wait and reset the value
            agent.skipNextPollTime = false;
        }
        else
        {
            // Otherwise, wait before asking for another job
            sleep(waitTime);
        }
    }

    return EXIT_SUCCESS;
}
