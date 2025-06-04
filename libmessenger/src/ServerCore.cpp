#include "ServerCore.hpp"

namespace messengerapp
{

ServerCore::ServerCore(boost::asio::io_context& io, int port) :
    io_(io),
    server_(std::make_shared<connection::server::Server>(io_, port))
{
}

ServerCore::~ServerCore()
{
    stopServer();
}

void ServerCore::startServer()
{
    server_->start();
}

void ServerCore::stopServer()
{
    server_->stop();
}

}  // namespace messengerapp
