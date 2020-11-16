#include <gtest/gtest.h>
#include "../src/Command.h"

string commandString = "echo \"Hello World\"";
string commandValue = "Hello World\n";
Command command(commandString);

TEST(CommandProcessTest, PipeSuccessfullyOpened)
{
    EXPECT_EQ(command.process(), EXIT_SUCCESS);
}

TEST(CommandProcessTest, CorrectCommandOutput)
{
    EXPECT_EQ(command.getOutput(), commandValue);
}