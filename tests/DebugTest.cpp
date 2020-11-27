#include <gtest/gtest.h>
#include <Agent.h>
#include <Debug.h>

class DebugTest : public ::testing::Test
{
    protected:
        DebugTest()
        {
        }

    public:
        Agent agent;
};

TEST_F(DebugTest, GetDebugModeTrue)
{
    Debug debug(true, this->agent.getAgentVersion());
    ASSERT_EQ(debug.getDebugMode(), true);
}

TEST_F(DebugTest, GetDebugModeFalse)
{
    Debug debug(false, this->agent.getAgentVersion());
    ASSERT_EQ(debug.getDebugMode(), false);
}

TEST_F(DebugTest, SetDebugModeFalse)
{
    Debug debug(true, this->agent.getAgentVersion());
    debug.setDebugMode(false);
    ASSERT_EQ(debug.getDebugMode(), false);
}

TEST_F(DebugTest, SetDebugModeTrue)
{
    Debug debug(false, this->agent.getAgentVersion());
    debug.setDebugMode(true);
    ASSERT_EQ(debug.getDebugMode(), true);
}

