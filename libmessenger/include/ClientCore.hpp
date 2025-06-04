#pragma once
#include <memory>
#include <string>
#include <functional>
#include <cstdint>
#include <thread>
#include <optional>

#include <boost/asio.hpp>

#include "gui/Event.hpp"
#include "messengerInterfaces/ILoginHub.hpp"
#include "messengerInterfaces/ILoginHubFactory.hpp"

#include "libmessenger/src/Login/AdminLoginHubCreator.hpp"
#include "libmessenger/src/Login/UserLoginHubCreator.hpp"
#include "libmessenger/src/Login/AdminLoginHub.hpp"
#include "libmessenger/src/Login/UserLoginHub.hpp"
#include "libmessenger/src/Login/LoginData.hpp"
#include "libmessenger/src/Registration/UserRegistrationHandler.hpp"
#include "libmessenger/src/Registration/AdminRegistrationHandler.hpp"
#include "libmessenger/src/Database/UserDatabaseController.hpp"
#include "libmessenger/src/Database/AdminDatabaseController.hpp"
#include "libmessenger/src/Connection/Client/Client.hpp"

namespace messengerapp
{

class ClientCore
{
public:
    ClientCore(
        boost::asio::io_context& io,
        int id,
        const std::string& userDatabasePath,
        const std::string& adminDatabasePath);

    boost::asio::io_context& io_;
    int id_;
    std::jthread clientThread_;
    std::optional<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> workGuard_;
    ChatEventHandler eventHandler_;

    std::string userDatabasePath_;
    std::string adminDatabasePath_;

    std::shared_ptr<connection::client::Client> client_;

    std::shared_ptr<database::UserDatabaseController> userDatabaseController_;
    std::shared_ptr<database::AdminDatabaseController> adminDatabaseController_;

    std::shared_ptr<registration::UserRegistrationHandler> userRegistrationHandler_;
    std::shared_ptr<registration::AdminRegistrationHandler> adminRegistrationHandler_;

    std::shared_ptr<login::UserLoginHub> userLoginHub_;
    std::shared_ptr<login::AdminLoginHub> adminLoginHub_;

    bool loginUser(const std::string& login, const std::string& password);
    bool loginAdmin(const std::string& login, const std::string& password);

    void registerUser(const std::string& login, const std::string& password);
    void registerAdmin(const std::string& login, const std::string& password);

    void startClient(const std::string& host, const std::string& port);
    void stopClient();
    void joinChat(const std::string& username);
    void leaveChat(const std::string& sender);
    void sendMessage(const std::string& sender, const std::string& recipient, const std::string& message);

    void setEventHandler(ChatEventHandler eventHandler);
};

}  // namespace messengerapp
