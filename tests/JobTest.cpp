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
    Job jobObj(debug, job, agent.getBakupJobConfirmationURL(), agent.getAuthToken(), false);
    ASSERT_EQ(jobObj.process(false), 0);
}

TEST_F(JobTest, ReportResults)
{
    // Create the command struct
    command_t job;
    job.id = "1";
    job.targetExecutionTime = time(NULL);
    job.commands.emplace_back("ls");

    // Construct the debug library for output
    Debug debug(true, agent.getAgentVersion());

    // Start the job process
    Job jobObj(debug, job, agent.getBakupJobConfirmationURL(), agent.getAuthToken(), false);
    jobObj.process(false);
    ASSERT_EQ(jobObj.reportResults(1, 1), true);
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
    Job jobObj(debug, job, agent.getBakupJobConfirmationURL(), agent.getAuthToken(), false);
    cpr::Error error = cpr::Error();
    string httpError = "HTTP ERROR";
    ASSERT_TRUE(jobObj.handleError(httpError, error));
}