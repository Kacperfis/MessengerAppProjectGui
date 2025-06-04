#include "MessageDecoder.hpp"

#include <vector>
#include <iostream>
#include <cstdint>

namespace connection::helpers::message
{

Message MessageDecoder::decodeMessage(const std::string& message) noexcept
{
    std::string messageType;
    std::string sender;
    std::string recipient;
    std::string content;

    std::vector<std::string> messageData;
    std::string currentData = "";
    for (uint32_t i = 0; i < message.size(); i++)
    {
        if (message[i] == '|')
        {
            messageData.push_back(currentData);
            currentData = "";
        }
        else currentData += message[i]; 
    }
    if (!currentData.empty()) messageData.push_back(currentData);
    
    if (messageData.size() == 1)
        return Message(messageData[0], "", "", "");
    else if (messageData.size() == 2)
        return Message(messageData[0], messageData[1], "", "");
    else if (messageData.size() == 3)
        return Message(messageData[0], messageData[1], messageData[2], "");
    else if (messageData.size() == 4)
        return Message(messageData[0], messageData[1], messageData[2], messageData[3]);
    else return Message("", "", "", "");
}

} // namespace connection::helpers::message
