#include "Message.hpp"

namespace connection::helpers::message
{

MessageType toMessageType(const std::string& message)
{
    if (message == "ESTABLISH")
    {
        return MessageType::ESTABLISH;
    }
    else if (message == "RELINQUISH")
    {
        return MessageType::RELINQUISH;
    }
    else if (message == "MESSAGE")
    {
        return MessageType::MESSAGE;
    }
    else if (message == "CHECK_AVAILABILITY")
    {
        return MessageType::CHECK_AVAILABILITY;
    }
    else
    {
        return MessageType::NONE;
    }
}

std::string toString(const MessageType messageType)
{
    switch (messageType)
    {
        case MessageType::ESTABLISH:
        {
            return "ESTABLISH";
        }
        case MessageType::RELINQUISH:
        {
            return "RELINQUISH";
        }
        case MessageType::MESSAGE:
        {
            return "MESSAGE";
        }
        case MessageType::CHECK_AVAILABILITY:
        {
            return "CHECK_AVAILABILITY";
        }
        case MessageType::NONE:
        {
            return "NONE";
        }
        default: return "NONE";
    }
}

} // namespace connection::helpers::message

