#include "Core.hpp"

namespace messengerapp
{

//====== Server Core ======

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

//====== Client Core ======

ClientCore::ClientCore(
    boost::asio::io_context& io,
    int id,
    const std::string& userDatabasePath,
    const std::string& adminDatabasePath) : 
    io_(io), 
    id_(id),
    userDatabasePath_(userDatabasePath),
    adminDatabasePath_(adminDatabasePath),
    client_(std::make_shared<connection::client::Client>(io_, id_)),
    userDatabaseController_(std::make_shared<database::UserDatabaseController>(userDatabasePath_)),
    adminDatabaseController_(std::make_shared<database::AdminDatabaseController>(adminDatabasePath_)),
    userRegistrationHandler_(std::make_shared<registration::UserRegistrationHandler>(userDatabaseController_)),
    adminRegistrationHandler_(std::make_shared<registration::AdminRegistrationHandler>(adminDatabaseController_)),
    userLoginHub_(std::make_shared<login::UserLoginHub>()),
    adminLoginHub_(std::make_shared<login::AdminLoginHub>())
{
    workGuard_.emplace(boost::asio::make_work_guard(io_));
}

bool ClientCore::loginUser(const std::string& login, const std::string& password)
{
    auto loginData = std::make_shared<login::LoginData>(login, password);
    return userLoginHub_->login(loginData, userRegistrationHandler_);
}

bool ClientCore::loginAdmin(const std::string& login, const std::string& password)
{
    auto loginData = std::make_shared<login::LoginData>(login, password);
    return adminLoginHub_->login(loginData, adminRegistrationHandler_);
}

bool ClientCore::registerUser(const std::string& login, const std::string& password)
{
   return userRegistrationHandler_->registerPerson(login, password);
}

bool ClientCore::registerAdmin(const std::string& login, const std::string& password)
{
    return adminRegistrationHandler_->registerPerson(login, password);
}

void ClientCore::startClient(const std::string& host, const std::string& port)
{
    if (clientThread_.joinable())
    {
        std::cout << "client thread is joinable" << std::endl;
        return;
    }
    client_->connect(host, port);
    clientThread_ = std::jthread([this]{
        client_->run();
    });
}

void ClientCore::stopClient()
{
    client_->stop();
    io_.stop();
    workGuard_.reset();
}

void ClientCore::joinChat(const std::string& recipient)
{
    client_->login(recipient);
}

void ClientCore::leaveChat(const std::string& sender)
{
    client_->logout(sender);
}

void ClientCore::sendMessage(const std::string& sender, const std::string& recipient, const std::string& message)
{
    client_->sendMessage(sender, recipient, message);
}

void ClientCore::setEventHandler(ChatEventHandler eventHandler)
{
    eventHandler_ = std::move(eventHandler);
    client_->setEventHandler(eventHandler_);
}

}  // namespace messengerapp