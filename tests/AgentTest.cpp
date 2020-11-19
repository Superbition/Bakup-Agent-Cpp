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
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            cout << "cwd: " << cwd << endl;
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
    cout << "fileName " << this->fileName << endl;
    cout << "fileContents " << this->fileContents << endl;
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