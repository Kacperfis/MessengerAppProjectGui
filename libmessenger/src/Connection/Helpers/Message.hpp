#pragma once

#include <string>
#include <iostream>

namespace connection::helpers::message
{

enum class MessageType
{
    ESTABLISH          = 1,
    RELINQUISH         = 2,
    MESSAGE            = 3,
    CHECK_AVAILABILITY = 4,
    NONE               = 5,
};

MessageType toMessageType(const std::string& message);
std::string toString(const MessageType messageType);

struct Message
{
    friend std::ostream& operator<<(std::ostream& os, const Message& message)
    {
        
        os << "\nMessageType: " << toString(message.messageType)
           << "\nSender: " << message.sender
           << "\nRecipient: " << message.recipient
           << "\nContent: " << message.content
           << "\nContent Size: " << message.contentSize;

        return os;
    }

    Message(const std::string& messageType_, const std::string& sender_, const std::string& recipient_, const std::string content_)
        : messageType(toMessageType(messageType_)), sender(sender_), recipient(recipient_), content(content_), contentSize(content.size()) {};

    MessageType messageType;
    std::string sender;
    std::string recipient;
    std::string content;
    int contentSize;
};

} // namespace connection::helpers::message
