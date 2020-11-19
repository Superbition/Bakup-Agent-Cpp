#include <gtest/gtest.h>
#include <Agent.h>

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
        const char *fileName = "testReadFile.txt";
        const string fileContents = "test";
};

// Test that a file can be read
TEST_F(AgentTest, ReadFile)
{
    ASSERT_EQ(AgentTest::agent.readFile(AgentTest::fileName), AgentTest::fileContents);
}

// Test if the User ID file was read
TEST_F(AgentTest, ReadUserId)
{
    ASSERT_NE(AgentTest::agent.getUserID(), "");
}

// Test if the auth token was read
TEST_F(AgentTest, ReadAuthToken)
{
    ASSERT_NE(AgentTest::agent.getAuthToken(), "");
}