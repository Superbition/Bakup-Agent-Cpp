#include <gtest/gtest.h>
#include "../src/Command.h"

string commandString = "pwd";
Command command(commandString);

TEST(CommandProcessTest, PipeSuccessfullyOpened)
{
    EXPECT_EQ(command.process(), EXIT_SUCCESS);
}