#pragma once

#include "Message.hpp"

#include <string>

namespace connection::helpers::message
{

class MessageDecoder final
{
public:
    static Message decodeMessage(const std::string& message) noexcept;
};

} // namespace connection::helpers::message
