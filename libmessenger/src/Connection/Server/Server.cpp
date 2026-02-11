#include "Server.hpp"

namespace connection::server
{

Server::Server(int port)
    : ioContext_()
    , acceptor_(ioContext_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    , activeSessions_(std::make_shared<session::SessionMap>())
    , sessionsMutex_(std::make_shared<std::mutex>())
    , logger_("Server") {}

Server::~Server()
{
    boost::system::error_code ec;
    acceptor_.close(ec);

    std::vector<std::shared_ptr<session::Session>> sessionsToStop;
    {
        std::lock_guard<std::mutex> lock(*sessionsMutex_);
        for (auto& [_, session] : *activeSessions_)
            sessionsToStop.push_back(session);
        activeSessions_->clear();
    }

    for (auto& session : sessionsToStop)
        session->stop();

    if (!ioContext_.stopped())
    {
        logger_.log(Severity::info, "stopping io_context");
        ioContext_.stop();
    }
}
void Server::acceptConnection()
{
    acceptor_.async_accept(
    [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
    {
        if (ec == boost::asio::error::operation_aborted) return;

        if (!ec)
        {
            auto session = std::make_shared<session::Session>(ioContext_, std::move(socket), activeSessions_, sessionsMutex_);
            session->start();
        }

        if (acceptor_.is_open())
            acceptConnection();
    });
}

void Server::start()
{
    logger_.log(Severity::info, "Server started on port 8080");
    std::cout << "Server started on port 8080" << std::endl;
    if (running_.exchange(true))
    {
        logger_.log(Severity::warning, "Server already running");
        return;
    }

    acceptConnection();
    ioThread_ = std::jthread([ctx = &ioContext_]
    {
        ctx->run();
    });
}

void Server::stop()
{
    boost::system::error_code ec;
    acceptor_.close(ec);
    if (ec)
    {
        logger_.log(Severity::warning, "Error closing acceptor: " + ec.message());
    }

    if (!ioContext_.stopped())
    {
        ioContext_.stop();
        running_ = false;
        logger_.log(Severity::info, "Server stopped");
        std::cout << "Server stopped" << std::endl;
    }
}

} // namespace connection::server
