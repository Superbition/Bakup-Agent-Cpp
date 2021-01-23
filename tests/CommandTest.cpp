#include <gtest/gtest.h>
#include <Command.h>

class CommandTest : public ::testing::Test
{
    protected:
        CommandTest()
        {
            this->commandString = "echo \"Hello World\"";
            this->commandValue = "Hello World";
        }

    public:
        string commandString;
        string commandValue;
};

TEST_F(CommandTest, PipeSuccessfullyOpened)
{
    Debug debug(true, "version");
    Command command(debug);
    ASSERT_TRUE(command.setupEnvironment());
}

TEST_F(CommandTest, CorrectCommandOutput)
{
    Debug debug(true, "version");
    Command command(debug);
    command.setupEnvironment();
    ASSERT_EQ(command.runCommand(this->commandString).first, this->commandValue);
}

TEST_F(CommandTest, GenerateDelimiter)
{
    Debug debug(true, "version");
    Command command(debug);
    string delimiter = command.generateDelimiter();
    ASSERT_EQ(delimiter.size(), 128);
}