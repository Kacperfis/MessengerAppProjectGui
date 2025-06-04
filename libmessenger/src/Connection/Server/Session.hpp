#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <map>
#include <set>
#include <string> 
#include <iostream>

#include "Encryption/EncryptionManager.hpp"
#include "Common/Logger.hpp"

namespace connection::session
{

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(boost::asio::ip::tcp::socket socket, std::map<std::string, std::shared_ptr<Session>>& sessions);
    void start();
private:
    std::string getActiveUsers(const std::set<std::string>& activeUsers, const std::string& username);
    void send(const std::string& data);
    void receive();

    std::shared_ptr<encryption::EncryptionManager> encryptionManager_;
    std::map<std::string, std::shared_ptr<Session>>& activeSessions_;
    
    std::string data_;
    std::string username_;

    boost::asio::ip::tcp::socket socket_;
    Logger logger_;  
};

} // namespace connection::session
