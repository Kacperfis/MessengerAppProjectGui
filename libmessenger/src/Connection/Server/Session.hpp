#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <map>
#include <set>
#include <deque>
#include <string> 
#include <iostream>
#include <mutex>

#include "Encryption/EncryptionManager.hpp"
#include "Common/Logger.hpp"

namespace connection::session
{

class Session;
using SessionMap = std::map<std::string, std::shared_ptr<Session>>;

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(boost::asio::io_context& io,
                     boost::asio::ip::tcp::socket socket,
                     std::shared_ptr<SessionMap> sessions,
                     std::shared_ptr<std::mutex> sessionsMutex);
    void start();
    void stop();
private:
    std::string getActiveUsers(const std::set<std::string>& activeUsers, const std::string& username);
    void send(const std::string& data);
    void receive();
    void doWrite();

    std::string getUsername() const;

    boost::asio::ip::tcp::socket socket_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;

    std::shared_ptr<SessionMap> activeSessions_;
    std::shared_ptr<std::mutex> sessionsMutex_;

    std::deque<std::string> writeQueue_;
    bool isWriting_ = false;
    
    std::string data_;

    mutable std::mutex usernameMutex_;
    std::string username_;

    Logger logger_;  
};

} // namespace connection::session
