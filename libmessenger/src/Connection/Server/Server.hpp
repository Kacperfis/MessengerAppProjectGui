#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#include "Session.hpp"
#include "Common/Logger.hpp"

namespace connection::server
{

class Server final
{
public:
    explicit Server(int port);
    ~Server();
    
    void start();
    void stop();
private:
    void acceptConnection();

    boost::asio::io_context ioContext_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::shared_ptr<session::SessionMap> activeSessions_;
    std::shared_ptr<std::mutex> sessionsMutex_;
    Logger logger_;
    std::atomic<bool> running_{false};
    std::jthread ioThread_;
};

} // namespace connection::server
