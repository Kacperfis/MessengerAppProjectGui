#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Connection/Helpers/MessageDecoder.hpp"

class MessageDecoderTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        messageDecoder_ = std::make_shared<connection::helpers::message::MessageDecoder>();
    }

    std::shared_ptr<connection::helpers::message::MessageDecoder> messageDecoder_;

    static inline const std::string messageType_ = "RELINQUISH";
    static inline const std::string sender_ = "sender";
    static inline const std::string recipient_ = "recipient";
    static inline const std::string content_ = "content";
};

TEST_F(MessageDecoderTests, shouldDecodeMessageWithSize1)
{
    const auto& message = messageType_ + "|||";
    const auto& result = messageDecoder_->decodeMessage(message);

    EXPECT_EQ(result.messageType, connection::helpers::message::MessageType::RELINQUISH);
    EXPECT_EQ(result.sender, "");
    EXPECT_EQ(result.recipient, "");
    EXPECT_EQ(result.content, "");
    EXPECT_EQ(result.contentSize, 0);
}

TEST_F(MessageDecoderTests, shouldDecodeMessageWithSize2)
{
    const auto& message = messageType_ + "|" + sender_ + "||";
    const auto& result = messageDecoder_->decodeMessage(message);

    EXPECT_EQ(result.messageType, connection::helpers::message::MessageType::RELINQUISH);
    EXPECT_EQ(result.sender, "sender");
    EXPECT_EQ(result.recipient, "");
    EXPECT_EQ(result.content, "");
    EXPECT_EQ(result.contentSize, 0);
}


TEST_F(MessageDecoderTests, shouldDecodeMessageWithSize3)
{
    const auto& message = messageType_ + "|" + sender_  + "|" + recipient_ + "|";
    const auto& result = messageDecoder_->decodeMessage(message);

    EXPECT_EQ(result.messageType, connection::helpers::message::MessageType::RELINQUISH);
    EXPECT_EQ(result.sender, "sender");
    EXPECT_EQ(result.recipient, "recipient");
    EXPECT_EQ(result.content, "");
    EXPECT_EQ(result.contentSize, 0);
}

TEST_F(MessageDecoderTests, shouldDecodeMessageWithSize4)
{
    const auto& message = messageType_+ "|" + sender_ + "|" + recipient_ + "|" + content_;
    const auto& result = messageDecoder_->decodeMessage(message);

    EXPECT_EQ(result.messageType, connection::helpers::message::MessageType::RELINQUISH);
    EXPECT_EQ(result.sender, "sender");
    EXPECT_EQ(result.recipient, "recipient");
    EXPECT_EQ(result.content, "content");
    EXPECT_EQ(result.contentSize, 7);
}
