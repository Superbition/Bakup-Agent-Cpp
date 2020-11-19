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
    Debug debug(true, agent);
    ASSERT_EQ(debug.getDebugMode(), true);
}

TEST_F(DebugTest, GetDebugModeFalse)
{
    Debug debug(false, agent);
    ASSERT_EQ(debug.getDebugMode(), false);
}

TEST_F(DebugTest, SetDebugModeFalse)
{
    Debug debug(true, agent);
    debug.setDebugMode(false);
    ASSERT_EQ(debug.getDebugMode(), false);
}

TEST_F(DebugTest, SetDebugModeTrue)
{
    Debug debug(false, agent);
    debug.setDebugMode(true);
    ASSERT_EQ(debug.getDebugMode(), true);
}

