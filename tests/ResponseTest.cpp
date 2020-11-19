#include <gtest/gtest.h>
#include <Agent.h>
#include <Response.h>

class ResponseTest : public ::testing::Test
{
    protected:
        ResponseTest()
        {

        }

    public:
        Agent agent;
};

TEST_F(ResponseTest, PostJobConfirmation)
{
    string jobStatus = "[{\"test\": true}]";
    string jobResponse;
    Response response(this->agent.getBakupJobConfirmationURL(), this->agent.getAuthToken());
    ASSERT_EQ(response.postJobConfirmation(jobStatus, jobResponse), 200);
}