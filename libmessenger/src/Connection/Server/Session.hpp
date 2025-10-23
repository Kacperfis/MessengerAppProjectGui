#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <map>
#include <set>
#include <string> 
#include <iostream>
#include <mutex>

#include "Encryption/EncryptionManager.hpp"
#include "Common/Logger.hpp"

namespace connection::session
{

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(boost::asio::io_context& io,
                     boost::asio::ip::tcp::socket socket, 
                     std::map<std::string, std::shared_ptr<Session>>& sessions,
                     std::mutex& sessionsMutex);
    void start();
private:
    std::string getActiveUsers(const std::set<std::string>& activeUsers, const std::string& username);
    void send(const std::string& data);
    void receive();

    boost::asio::ip::tcp::socket socket_;
    std::map<std::string, std::shared_ptr<Session>>& activeSessions_;
    std::mutex& sessionsMutex_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    std::shared_ptr<encryption::EncryptionManager> encryptionManager_;
    
    std::string data_;
    std::string username_;
    Logger logger_;  
};

} // namespace connection::session
