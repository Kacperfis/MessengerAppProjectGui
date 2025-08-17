#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "src/Connection/Helpers/MessageHandler.hpp"
#include "mocks/EncryptionManagerMock.hpp"
#include "gui/Event.hpp"

using testing::_;
using testing::Eq;
using testing::Field;
using testing::AllOf;
using testing::MockFunction;
using testing::Return;
using testing::SaveArg;
using testing::StrictMock;
using testing::Invoke;

MATCHER_P4(MessageEq, type, sender, recipient, message, "")
{
    *result_listener << "where type = " << arg.type
        << ", sender = " << arg.sender
        << ", recipient = " << arg.recipient
        << ", message = " << arg.message;

    return arg.type == type &&
        arg.sender == sender &&
        arg.recipient == recipient &&
        arg.message == message;
}

class MessageHandlerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        messageHandler_ = std::make_shared<connection::helpers::message::MessageHandler>();
    }

    StrictMock<MockFunction<void(const ChatEvent&)>> chatEventCallback_;
    std::shared_ptr<connection::helpers::message::MessageHandler> messageHandler_;
};

TEST_F(MessageHandlerTest, shouldHandleCheckAvailabilityMessageWithNoActiveUsers)
{
    connection::helpers::message::Message message("CHECK_AVAILABILITY", "Alice", "Bob", " ");

    EXPECT_CALL(chatEventCallback_, Call(MessageEq(ChatEvent::Type::CHECK_AVAILABILITY, "", "", "")));

    testing::internal::CaptureStdout();
    messageHandler_->handleMessage(message, chatEventCallback_.AsStdFunction());
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "[INFO] Alice has joined to the chat\n[INFO] there are no active users\n");
}

TEST_F(MessageHandlerTest, shouldHandleCheckAvailabilityMessageWithActiveUsers)
{
    connection::helpers::message::Message message("CHECK_AVAILABILITY", "Alice", "Bob", "Alice");

    EXPECT_CALL(chatEventCallback_, Call(MessageEq(ChatEvent::Type::CHECK_AVAILABILITY, "", "", "")));

    testing::internal::CaptureStdout();
    messageHandler_->handleMessage(message, chatEventCallback_.AsStdFunction());
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "[INFO] Alice has joined to the chat\n[INFO] current active users: Alice\n");
}

TEST_F(MessageHandlerTest, shouldHandleStandardMessage)
{
    connection::helpers::message::Message message("MESSAGE", "Alice", "", "content");

    EXPECT_CALL(chatEventCallback_, Call(MessageEq(ChatEvent::Type::STANDARD_MESSAGE, "Alice", "", "content")));

    testing::internal::CaptureStdout();
    messageHandler_->handleMessage(message, chatEventCallback_.AsStdFunction());
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "[Alice]: content\n");
}
