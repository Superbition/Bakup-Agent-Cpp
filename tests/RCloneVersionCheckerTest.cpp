#include <gtest/gtest.h>
#include <RCloneVersionChecker.h>

class RCloneVersionCheckerTest : public ::testing::Test
{
    protected:
        RCloneVersionCheckerTest()
        {

        }

        ~RCloneVersionCheckerTest()
        {

        }
};

TEST_F(RCloneVersionCheckerTest, GetRCloneVersion)
{
    RCloneVersionChecker rCloneVersionChecker;

    string version = rCloneVersionChecker.getRCloneVersion();

    ASSERT_NE(version, "");
}