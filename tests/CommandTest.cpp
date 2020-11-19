#include <gtest/gtest.h>
#include <Command.h>

class CommandTest : public ::testing::Test
{
    protected:
        CommandTest()
        {
            this->commandString = "echo \"Hello World\"";
            this->commandValue = "Hello World\n";
        }

    public:
        string commandString;
        string commandValue;
};

TEST_F(CommandTest, PipeSuccessfullyOpened)
{
    Command command(this->commandString);
    ASSERT_EQ(command.process(), EXIT_SUCCESS);
}

TEST_F(CommandTest, CorrectCommandOutput)
{
    Command command(this->commandString);
    command.process();
    ASSERT_EQ(command.getOutput(), this->commandValue);
}