#include <gtest/gtest.h>
#include <Agent.h>
#include <Debug.h>
#include <cpr/cpr.h>

#include <fstream>
#include <cstdio>


class AgentTest : public ::testing::Test
{
    protected:
        // Before the tests are run create a file for readFile test
        AgentTest()
        {
            // Write a file to disk to test the readFile function
            ofstream testFile;
            testFile.open(fileName);
            testFile << fileContents;
            testFile.close();
        }

        // Once the tests are finished, or failed, delete the test file
        ~AgentTest()
        {
            // Delete the file
            std::remove(fileName);
        }

    public:
        // Create an agent class
        Agent agent;

        // File name and contents
        const char *fileName = "/tmp/testReadFile.txt";
        const string fileContents = "test";
};

// Test that a file can be read
TEST_F(AgentTest, ReadFile)
{
    ASSERT_EQ(this->agent.readFile(this->fileName), this->fileContents);
}

// Test if the User ID file was read
TEST_F(AgentTest, ReadUserId)
{
    ASSERT_NE(this->agent.getUserID(), "");
}

// Test if the Client ID file was read
TEST_F(AgentTest, ReadClientId)
{
    ASSERT_NE(this->agent.getClientId(), "");
}

// Test if the api token was read
TEST_F(AgentTest, ReadApiToken)
{
    ASSERT_NE(this->agent.getApiToken(), "");
}

// Test that error text can be processed
TEST_F(AgentTest, HandleErrors)
{
    // Create the debug class
    Debug debug(true, agent.getAgentVersion());
    cpr::Error error = cpr::Error();
    ASSERT_TRUE(this->agent.handleError(debug, "HTTP ERROR", error));
}

// Test resetting agent class
TEST_F(AgentTest, ResetAgentVariables)
{
    // Create the debug class
    Debug debug(true, agent.getAgentVersion());
    ASSERT_TRUE(this->agent.resetJob(debug));
}

TEST_F(AgentTest, RefreshAgentCredentials)
{
    const string newClientId = "NEW_CLIENT_ID";
    const string newApiToken = "NEW_API_TOKEN";

    // Update the client ID file to the new value
    ofstream clientIdFile;
    clientIdFile.open("/etc/opt/bakupagent/CLIENT_ID");
    clientIdFile << newClientId << endl;
    clientIdFile.close();

    // Update the api token file to the new value
    ofstream apiTokenFile;
    apiTokenFile.open("/etc/opt/bakupagent/API_TOKEN");
    apiTokenFile << newApiToken << endl;
    apiTokenFile.close();

    // Trigger the agent to refresh credentials
    Debug debug(true, agent.getAgentVersion());
    agent.refreshAgentCredentials(debug);

    // Check the agent has the new authentication values
    ASSERT_EQ(agent.getClientId(), newClientId);
    ASSERT_EQ(agent.getApiToken(), newApiToken);
}