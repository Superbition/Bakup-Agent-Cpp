#include <gtest/gtest.h>
#include <Command.h>
#include <Agent.h>

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
        Agent agent;
};

TEST_F(CommandTest, PipeSuccessfullyOpened)
{
    Debug debug(true, "version");
    Command command(debug, this->agent.getUserID());
    ASSERT_TRUE(command.setupEnvironment());
}

TEST_F(CommandTest, PipeFailsOnInvalidShell)
{
    Debug debug(true, "version");

    // Pass an invalid shell that crashes child
    Command command(debug, this->agent.getUserID(), "ABadShell");

    ASSERT_FALSE(command.setupEnvironment());
}

TEST_F(CommandTest, PipeFailsOnBadShell)
{
    Debug debug(true, "version");

    // Pass an invalid shell that doesn't crash the child
    Command command(debug, this->agent.getUserID(), "/bin/touch");

    ASSERT_FALSE(command.setupEnvironment());
}

TEST_F(CommandTest, CommandSuccessTest)
{
    Debug debug(true, "version");
    Command command(debug, this->agent.getUserID());
    command.setupEnvironment();
    ASSERT_EQ(command.runCommand(this->commandString).first, this->commandValue);
}

TEST_F(CommandTest, GenerateDelimiter)
{
    Debug debug(true, "version");
    Command command(debug, this->agent.getUserID());
    string delimiter = command.generateDelimiter();
    ASSERT_EQ(delimiter.size(), 128);
}

TEST_F(CommandTest, CommandFailureTest)
{
    Debug debug(true, "version");
    Command command(debug, this->agent.getUserID());
    command.setupEnvironment();
    ASSERT_GT(command.runCommand("notAValidCommand").second, EXIT_SUCCESS);
}

TEST_F(CommandTest, WhoAmIBakupAgent)
{
    Debug debug(true, "version");
    Command command(debug, this->agent.getUserID());
    command.setupEnvironment();
    ASSERT_EQ(command.runCommand("whoami").first, "bakupagent");
}