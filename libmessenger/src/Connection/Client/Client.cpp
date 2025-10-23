#include "Client.hpp"
#include <iostream>
#include <thread>
#include <memory>
#include <syncstream>

#include "Connection/Helpers/Message.hpp"
#include "Connection/Helpers/MessageDecoder.hpp"
#include "Connection/Helpers/MessageHandler.hpp"

namespace connection::client
{

Client::Client(boost::asio::io_context& io, int id) :
    ioContext_(io),
    strand_(boost::asio::make_strand(io)),
    id_(id),
    socket_(ioContext_),
    resolver_(ioContext_),
    encryptionManager_(std::make_shared<encryption::EncryptionManager>("", "0123456789abcdef0123456789abcdef", "0123456789abcdef")),
    logger_("Client") {}

void Client::setEventHandler(ChatEventHandler eventHandler)
{
    eventHandler_ = std::move(eventHandler);
}

void Client::connect(const std::string& host, const std::string& port)
{
    std::osyncstream(std::cout) << "[" << id_ << "]client connect\n";
    auto self = shared_from_this();
    boost::asio::connect(socket_, resolver_.resolve(host, port));
    boost::asio::post(strand_, [this, self]() { readData(); });
}

void Client::readData()
{
    auto self = shared_from_this();
    boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(data_), '\n',
        boost::asio::bind_executor(strand_,
        [this, self](boost::system::error_code errorCode, std::size_t length)
        {
            if (!errorCode)
            {
                logger_.log(Severity::info, "received " + std::to_string(length) + " bytes of data");
                std::cout <<  "client received " + std::to_string(length) + " bytes of data" << std::endl;

                std::string message;
                {
                    std::lock_guard<std::mutex> lock(mtx_);
                    message = data_.substr(0, length);
                    data_.erase(0, length);
                }

                logger_.log(Severity::info, "received message: " + message);
                auto decodedMessage = helpers::message::MessageDecoder::decodeMessage(message);
                helpers::message::MessageHandler::handleMessage(decodedMessage, eventHandler_);
                readData();
            }
            else if (errorCode == boost::asio::error::operation_aborted)
            {
                logger_.log(Severity::warning, "read operation was cancelled");
            }
            else
            {
                logger_.log(Severity::warning, "read operation failed with error: " + errorCode.message());
            }
        })
    );
}

void Client::sendData(const std::string& data)
{
    logger_.log(Severity::info, "sending " + data);
    std::cout << "client sending " + data << std::endl;

    auto self = shared_from_this();
    std::string message = data + '\n';

    boost::asio::post(strand_,
    [self, msg = std::move(message)]()
    {
        boost::asio::async_write(self->socket_,
            boost::asio::buffer(msg),
            boost::asio::bind_executor(self->strand_,
            [self, msg](const boost::system::error_code& errorCode, std::size_t)
            {
                if (errorCode)
                    self->logger_.log(Severity::warning, "async write failed: " + errorCode.message());
                else
                    self->logger_.log(Severity::info, "message sent successfully");
            }));
    });
}

void Client::login(const std::string& recipient)
{
    sendData("ESTABLISH|" + recipient + "||");
}

void Client::sendMessage(const std::string& sender, const std::string& recipient, const std::string& message)
{
    sendData("MESSAGE|" + sender + "|" + recipient + "|" + message);
}

void Client::logout(const std::string& sender)
{
    sendData("RELINQUISH|" + sender + "||");
}

void Client::run()
{
    logger_.log(Severity::info, "client started");
    std::osyncstream(std::cout) << "[" << id_ << "]client started\n";
    ioContext_.run();
}

void Client::stop()
{
    std::osyncstream(std::cout) << "[" << id_ << "]client stopped\n";

    auto self = shared_from_this();
    boost::asio::post(strand_,
    [this, self]()
    {
        boost::system::error_code errorCode;

        if (socket_.is_open())
        {
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, errorCode);
            if (errorCode)
            {
                logger_.log(Severity::error, "Error on socket shutdown during disconnect: " + errorCode.message());
                std::cerr << "Error on socket shutdown during disconnect: " << errorCode.message() << std::endl;
            }

            socket_.close(errorCode);
            if (errorCode)
            {
                logger_.log(Severity::error, "Error on socket close during disconnect: " + errorCode.message());
                std::cerr << "Error on socket close during disconnect: " << errorCode.message() << std::endl;
            }
        }
    });
}

} // namespace connection::client
