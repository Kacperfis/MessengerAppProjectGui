#pragma once

#include <boost/asio.hpp>
#include <string>
#include <thread>

#include "Encryption/EncryptionManager.hpp"
#include "Common/Logger.hpp"
#include <gui/Event.hpp>

namespace connection::client
{

class Client : public std::enable_shared_from_this<Client>
{
public:
    Client(boost::asio::io_context& io, int id);

    void setEventHandler(ChatEventHandler handler);
    void connect(const std::string& host, const std::string& port);
    void login(const std::string& recipient);
    void logout(const std::string& sender);
    void sendMessage(const std::string& sender, const std::string& recipient, const std::string& message);

    void run();
    void stop();

private:
    void readData();
    void sendData(const std::string& data);

    boost::asio::io_context& ioContext_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::resolver resolver_;
    std::shared_ptr<encryption::EncryptionManager> encryptionManager_;

    ChatEventHandler eventHandler_;

    int id_;
    std::string data_;
    std::mutex mtx_;
    Logger logger_;
};

} // namespace connection::client