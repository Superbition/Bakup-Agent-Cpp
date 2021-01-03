#include <gtest/gtest.h>

#include <ResponseBuilder.h>
#include <vector>

class ResponseBuilderTest : public ::testing::Test
{
    protected:
        ResponseBuilderTest()
        {

        }

        ~ResponseBuilderTest()
        {

        }

    public:

};

TEST_F(ResponseBuilderTest, BuildJson)
{
    ResponseBuilder responseBuilder;
    ASSERT_EQ(responseBuilder.build(), "{}");
}

TEST_F(ResponseBuilderTest, AddErrorCode)
{
    ResponseBuilder responseBuilder;
    responseBuilder.addErrorCode(0);
    ASSERT_EQ(responseBuilder.build(), "{\"error_code\":0}");
}

TEST_F(ResponseBuilderTest, AddErrorMessage)
{
    ResponseBuilder responseBuilder;
    responseBuilder.addErrorMessage("test");
    ASSERT_EQ(responseBuilder.build(), "{\"error_message\":\"test\"}");
}

TEST_F(ResponseBuilderTest, AddSendAttempt)
{
    ResponseBuilder responseBuilder;
    responseBuilder.addSendAttempt(0);
    ASSERT_EQ(responseBuilder.build(), "{\"send_attempt\":0}");
}

TEST_F(ResponseBuilderTest, AddCommandOutputs)
{
    ResponseBuilder responseBuilder;
    vector<commandOutput> commandsOutput;
    commandOutput temp;
    temp.command = "test_command";
    temp.statusCode = 0;
    temp.result = "test_result";
    commandsOutput.push_back(temp);
    commandsOutput.push_back(temp);
    responseBuilder.addCommandOutputs(commandsOutput);

    // Build and test string
    ASSERT_EQ(responseBuilder.build(), "{\"command_output\":[{\"command\":\"test_command\",\"status_code\":0,\"result\":\"test_result\"},{\"command\":\"test_command\",\"status_code\":0,\"result\":\"test_result\"}]}");
}