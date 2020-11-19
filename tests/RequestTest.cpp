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
    Request request(RequestTest::agent.getBakupRequestURL(), RequestTest::agent.getAuthToken());
    ASSERT_EQ(request.getBakupJob(), 200);
}