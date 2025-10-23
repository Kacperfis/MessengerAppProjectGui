#include "Server.hpp"

namespace connection::server
{

Server::Server(boost::asio::io_context& io, int port)
    : ioContext_(io)
    , acceptor_(ioContext_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    , logger_("Server") {}

Server::~Server()
{
    stop();
}

void Server::acceptConnection()
{
    acceptor_.async_accept(
    [this](boost::system::error_code errorCode, boost::asio::ip::tcp::socket socket)
    {
        if (!errorCode)
        {
            auto session = std::make_shared<session::Session>(ioContext_, std::move(socket), activeSessions_, sessionsMutex_);
            session->start();
        }
        acceptConnection();
    });
}

void Server::start()
{
    logger_.log(Severity::info, "Server started on port 8080");
    std::cout << "Server started on port 8080" << std::endl;
    acceptConnection();
    ioThread_ = std::jthread([ctx = &ioContext_]
    {
        ctx->run();
    });
}

void Server::stop()
{
    if (!ioContext_.stopped())
    {
        ioContext_.stop();
        logger_.log(Severity::info, "Server stopped");
        std::cout << "Server stopped" << std::endl;
    }
}

} // namespace connection::server
