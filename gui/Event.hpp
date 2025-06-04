#pragma once

#include <vector>
#include <string>
#include <functional>

struct ChatEvent
{
    enum Type {CHECK_AVAILABILITY, STANDARD_MESSAGE} type;

    // check availability
    std::vector<std::string> activeUsers;
    std::string newlyJoinedUser;

    // standard message
    std::string sender;
    std::string recipient;
    std::string message;
};

using ChatEventHandler = std::function<void(const ChatEvent&)>;
