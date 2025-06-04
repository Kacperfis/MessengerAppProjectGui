#pragma once

#include <memory>

#include "gui/Event.hpp"
#include "Message.hpp"
#include "Common/Logger.hpp"
#include "messengerInterfaces/IEncryptionManager.hpp"

namespace connection::helpers::message
{

class MessageHandler final
{
public:
    static void handleMessage(
        const Message& message, const std::shared_ptr<interface::IEncryptionManager>& encryptionManager, const ChatEventHandler& eventHandler) noexcept;
private:
    static void handleCheckAvailabilityMessage(const Message& message, const ChatEventHandler& eventHandler) noexcept;
    static void handleStandardMessage(
        const Message& message, const std::shared_ptr<interface::IEncryptionManager>& encryptionManager, const ChatEventHandler& eventHandler) noexcept;
    static std::vector<std::string> extractActiveUsers(std::string_view input) noexcept;

};

} // namespace connection::helpers::message
