// #include <gtest/gtest.h>
// #include <gmock/gmock.h>

// #include "src/Connection/Helpers/MessageHandler.hpp"
// #include "mocks/EncryptionManagerMock.hpp"

// using ::testing::_;
// using ::testing::Return;

// class MessageHandlerTest : public ::testing::Test
// {
// protected:
//     void SetUp() override
//     {
//         encryptionManagerMock_ = std::make_shared<mocks::EncryptionManagerMock>();
//         messageHandler_ = std::make_shared<connection::helpers::message::MessageHandler>();
//     }

//     std::shared_ptr<connection::helpers::message::MessageHandler> messageHandler_;
//     std::shared_ptr<mocks::EncryptionManagerMock> encryptionManagerMock_;
// };

// TEST_F(MessageHandlerTest, shouldHandleCheckAvailabilityMessageWithNoActiveUsers)
// {
//     connection::helpers::message::Message message("CHECK_AVAILABILITY", "Alice", "Bob", " ");

//     testing::internal::CaptureStdout();
//     messageHandler_->handleMessage(message, encryptionManagerMock_);
//     std::string output = testing::internal::GetCapturedStdout();

//     EXPECT_EQ(output, "[INFO] Alice has joined to the chat\n[INFO] there are no active users\n");
// }


// TEST_F(MessageHandlerTest, shouldHandleCheckAvailabilityMessageWithActiveUsers)
// {
//     connection::helpers::message::Message message("CHECK_AVAILABILITY", "Alice", "Bob", "Alice");

//     testing::internal::CaptureStdout();
//     messageHandler_->handleMessage(message, encryptionManagerMock_);
//     std::string output = testing::internal::GetCapturedStdout();

//     EXPECT_EQ(output, "[INFO] Alice has joined to the chat\n[INFO] current active users: Alice\n");
// }

// TEST_F(MessageHandlerTest, shouldHandleStandardMessage)
// {
//     connection::helpers::message::Message message("MESSAGE", "Alice", "Bob", "encrypted message");

//     EXPECT_CALL(*encryptionManagerMock_, decryptString("encrypted message"))
//         .WillOnce(Return("decrypted message"));

//     testing::internal::CaptureStdout();
//     messageHandler_->handleMessage(message, encryptionManagerMock_);
//     std::string output = testing::internal::GetCapturedStdout();

//     EXPECT_EQ(output, "[Alice]: decrypted message\n");
// }
