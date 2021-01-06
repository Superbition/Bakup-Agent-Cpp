#include <gtest/gtest.h>
#include <Job.h>
#include <Agent.h>
#include <Debug.h>
#include <Request.h>
#include <ctime>

class JobTest : public ::testing::Test
{
    protected:
        JobTest()
        {
        }

        ~JobTest()
        {
        }

    public:
        Agent agent;
};

TEST_F(JobTest, ProcessTest)
{
    // Create the command struct
    command_t job;
    job.id = "1";
    job.targetExecutionTime = time(NULL);
    job.commands.emplace_back("ls");

    // Construct the debug library for output
    Debug debug(true, agent.getAgentVersion());

    // Start the job process
    Job jobObj(debug, job, agent.getBakupJobConfirmationURL(), agent.getClientId(), agent.getApiToken(), false);
    ASSERT_EQ(jobObj.process(false), 0);
}

TEST_F(JobTest, FailProcessTest)
{
    // Create the command struct with an invalid command
    command_t job;
    job.id = "1";
    job.targetExecutionTime = time(NULL);
    job.commands.emplace_back("notAValidCommand");

    // Construct the debug library for output
    Debug debug(true, agent.getAgentVersion());

    // Start the job process
    Job jobObj(debug, job, agent.getBakupJobConfirmationURL(), agent.getClientId(), agent.getApiToken(), false);
    ASSERT_GT(jobObj.process(false), 0);
}

TEST_F(JobTest, HandleErrors)
{
    // Create the command struct
    command_t job;
    job.id = "1";
    job.targetExecutionTime = time(NULL);
    job.commands.emplace_back("ls");

    // Construct the debug library for output
    Debug debug(true, agent.getAgentVersion());

    // Start the job process
    Job jobObj(debug, job, agent.getBakupJobConfirmationURL(), agent.getClientId(), agent.getApiToken(), false);
    cpr::Error error = cpr::Error();
    string httpError = "HTTP ERROR";
    ASSERT_TRUE(jobObj.handleError(httpError, error));
}