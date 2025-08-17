#include "MessageHandler.hpp"

#include <vector>
#include <iostream>
#include <memory>
#include <ranges>

namespace connection::helpers::message
{

void MessageHandler::handleMessage(
    const Message& message,
    const ChatEventHandler& eventHandler) noexcept
{
    Logger logger("MessageHandler");
    logger.log(Severity::info, "handling message with messageType: " + toString(message.messageType));
    switch (message.messageType)
    {
        case MessageType::CHECK_AVAILABILITY:
        {
            handleCheckAvailabilityMessage(message, eventHandler);
            break;
        }
        case MessageType::MESSAGE:
        {
            handleStandardMessage(message, eventHandler);
            break;
        }
    }
}

void MessageHandler::handleCheckAvailabilityMessage(const Message& message, const ChatEventHandler& eventHandler) noexcept
{
    ChatEvent event {ChatEvent::CHECK_AVAILABILITY};
    event.activeUsers = extractActiveUsers(message.content);

    if (message.sender != message.recipient)
    {
        std::cout << "[INFO] " << message.sender << " has joined to the chat" << std::endl;
        event.newlyJoinedUser = message.sender;
    }
    if (message.contentSize-1 == 0)
    {
        std::cout << "[INFO] there are no active users" << std::endl;
    }
    else
    {
        std::cout << "[INFO] current active users: " << message.content << std::endl;
    }

    if (eventHandler)
        eventHandler(event);
}

void MessageHandler::handleStandardMessage(
    const Message& message,
    const ChatEventHandler& eventHandler) noexcept
{
    ChatEvent event {ChatEvent::STANDARD_MESSAGE};
    event.sender = message.sender;
    event.message = message.content;
    std::cout << "[" << message.sender << "]: " << message.content << std::endl;

    if (eventHandler)
        eventHandler(event);
}

std::vector<std::string> MessageHandler::extractActiveUsers(std::string_view input) noexcept
{
    std::vector<std::string> users;
    std::string token;

    std::stringstream ss(std::string{input});
    while (std::getline(ss, token, ','))
    {
        if (!token.empty())
            users.push_back(token);
    }
    return users;
}

} // namespace connection::helpers::message
