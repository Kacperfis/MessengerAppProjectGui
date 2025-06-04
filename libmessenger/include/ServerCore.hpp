#pragma once
#include <memory>
#include <boost/asio.hpp>

#include "libmessenger/src/Connection/Server/Server.hpp"

namespace messengerapp
{

class ServerCore
{
public:
    ServerCore(
        boost::asio::io_context& io,
        int port);
    ~ServerCore();

    boost::asio::io_context& io_;
    std::shared_ptr<connection::server::Server> server_;

    void startServer();
    void stopServer();
};

}  // namespace messengerapp
