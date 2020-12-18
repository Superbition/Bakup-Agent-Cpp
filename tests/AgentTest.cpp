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

// Test if the auth token was read
TEST_F(AgentTest, ReadAuthToken)
{
    ASSERT_NE(this->agent.getAuthToken(), "");
}

// Test that error text can be processed
TEST_F(AgentTest, HandleErrors)
{
    // Create the debug class
    Debug debug(true, agent.getAgentVersion());
    cpr::Error error = cpr::Error();
    ASSERT_TRUE(this->agent.handleError(debug, "HTTP ERROR", error));
}

// Test retrieving job
TEST_F(AgentTest, GettingJob)
{
    // Create the debug class
    Debug debug(true, agent.getAgentVersion());
    ASSERT_TRUE(this->agent.getJob(debug));
}

// Test resetting agent class
TEST_F(AgentTest, ResetAgentVariables)
{
    // Create the debug class
    Debug debug(true, agent.getAgentVersion());
    ASSERT_TRUE(this->agent.resetJob(debug));
}