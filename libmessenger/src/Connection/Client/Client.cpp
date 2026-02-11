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
    logger_("Client") {}

void Client::setEventHandler(ChatEventHandler eventHandler)
{
    std::lock_guard<std::mutex> lock(eventHandlerMutex_);
    eventHandler_ = std::move(eventHandler);
}

void Client::connect(const std::string& host, const std::string& port)
{
    boost::system::error_code ec;
    auto endpoints = resolver_.resolve(host, port, ec);
    if (ec) {
        logger_.log(Severity::error, "[" + std::to_string(id_) + "] DNS resolution failed: " + ec.message());
        return;
    }

    boost::asio::connect(socket_, endpoints, ec);
    if (ec) {
        logger_.log(Severity::error, "[" + std::to_string(id_) + "] Connection failed: " + ec.message());
        return;
    }

    auto self = shared_from_this();
    boost::asio::post(strand_, [this, self]() { readData(); });
}

void Client::readData()
{
    auto self = shared_from_this();
    boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(data_, 1024*1024), '\n',
        boost::asio::bind_executor(strand_,
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                logger_.log(Severity::info, "[" + std::to_string(id_) + "] received " + std::to_string(length) + " bytes of data");
                std::cout << "[" << id_ << "] client received " << length << " bytes of data" << std::endl;

                std::string message = data_.substr(0, length);
                data_.erase(0, length);

                logger_.log(Severity::info, "[" + std::to_string(id_) + "] received message: " + message);
                auto decodedMessage = helpers::message::MessageDecoder::decodeMessage(message);
                
                ChatEventHandler handlerCopy;
                {
                    std::lock_guard<std::mutex> lock(eventHandlerMutex_);
                    handlerCopy = eventHandler_;
                }
                
                helpers::message::MessageHandler::handleMessage(decodedMessage, handlerCopy);
                readData();
            }
            else if (ec == boost::asio::error::operation_aborted)
            {
                logger_.log(Severity::warning, "[" + std::to_string(id_) + "] read operation was cancelled");
            }
            else
            {
                logger_.log(Severity::warning, "[" + std::to_string(id_) + "] read operation failed with error: " + ec.message());
            }
        })
    );
}

void Client::sendData(const std::string& data)
{
    logger_.log(Severity::info, "[" + std::to_string(id_) + "] sending: " + data);
    std::cout << "[" << id_ << "] client sending: " << data << std::endl;

    auto self = shared_from_this();
    auto msg = std::make_shared<std::string>(data + '\n');

    boost::asio::post(strand_,
    [this, self, msg]()
    {
        writeQueue_.push_back(*msg);
        if (!isWriting_)
        {
            doWrite();
        }
    });
}

void Client::doWrite()
{
    if (writeQueue_.empty())
    {
        isWriting_ = false;
        return;
    }

    isWriting_ = true;
    auto& msg = writeQueue_.front();
    auto self = shared_from_this();

     boost::asio::async_write(self->socket_, boost::asio::buffer(msg),
        boost::asio::bind_executor(self->strand_,
        [this, self](const boost::system::error_code& ec, std::size_t)
        {
            writeQueue_.pop_front();
            if (ec)
                self->logger_.log(Severity::warning, "[" + std::to_string(id_) + "] async write failed: " + ec.message());
            else
            {
                self->logger_.log(Severity::info, "[" + std::to_string(id_) + "] message sent successfully");
                doWrite();
            }
        }));
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
    logger_.log(Severity::info, "[" + std::to_string(id_) + "] client started");
    std::osyncstream(std::cout) << "[" << id_ << "] client started\n";
    ioContext_.run();
}

void Client::stop()
{
    std::osyncstream(std::cout) << "[" << id_ << "] client stopped\n";

    auto self = shared_from_this();
    boost::asio::post(strand_,
    [this, self]()
    {
        boost::system::error_code ec;

        if (socket_.is_open())
        {
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
            if (ec)
            {
                logger_.log(Severity::error, "[" + std::to_string(id_) + "] Error on socket shutdown during disconnect: " + ec.message());
                std::cerr << "[" << id_ << "] Error on socket shutdown during disconnect: " << ec.message() << std::endl;
            }

            socket_.close(ec);
            if (ec)
            {
                logger_.log(Severity::error, "[" + std::to_string(id_) + "] Error on socket close during disconnect: " + ec.message());
                std::cerr << "[" << id_ << "] Error on socket close during disconnect: " << ec.message() << std::endl;
            }
        }
    });
}

} // namespace connection::client
