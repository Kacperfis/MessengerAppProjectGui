#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

#include "src/Connection/Helpers/Message.hpp"
#include "src/Common/Logger.hpp"
#include "common/commonFixture.hpp"

using testing::ElementsAre;
using testing::Return;
using testing::_;

TEST(MessageTests, shouldConvertStringMessageToEnum)
{
    const std::vector<std::string> messages =
        {"ESTABLISH", "RELINQUISH", "MESSAGE", "CHECK_AVAILABILITY"};

    std::vector<connection::helpers::message::MessageType> result;
    for (const auto& message : messages)
    {
        result.push_back(connection::helpers::message::toMessageType(message));
    }

    EXPECT_THAT(result, ElementsAre(
        connection::helpers::message::MessageType::ESTABLISH,
        connection::helpers::message::MessageType::RELINQUISH,
        connection::helpers::message::MessageType::MESSAGE,
        connection::helpers::message::MessageType::CHECK_AVAILABILITY));
}

TEST(MessageTests, shouldConvertEnumToStringMessage)
{
    const std::vector<connection::helpers::message::MessageType> messages =
        {connection::helpers::message::MessageType::ESTABLISH,
         connection::helpers::message::MessageType::RELINQUISH,
         connection::helpers::message::MessageType::MESSAGE,
         connection::helpers::message::MessageType::CHECK_AVAILABILITY};

    std::vector<std::string> result;
    for (const auto& message : messages)
    {
        result.push_back(connection::helpers::message::toString(message));
    }

    EXPECT_THAT(result, ElementsAre("ESTABLISH", "RELINQUISH", "MESSAGE", "CHECK_AVAILABILITY"));
}
