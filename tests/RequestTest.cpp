#include <gtest/gtest.h>
#include <Agent.h>
#include <Request.h>

class RequestTest : public ::testing::Test
{
    protected:
        RequestTest()
        {

        }

    public:
        Agent agent;
        string response;
};

TEST_F(RequestTest, MakeWebRequest)
{
    Request request(this->agent.getBakupRequestURL(), this->agent.getAuthToken());
    int statusCode = request.getBakupJob();
    ASSERT_GE(statusCode, 200);
    ASSERT_LT(statusCode, 500);
}