#pragma once
#include <memory>
#include <string>
#include <iostream>

#include "Database/AdminDatabaseController.hpp"
#include "messengerInterfaces/IRegistrationHandler.hpp"
#include "Login/LoginData.hpp"

namespace registration
{

class AdminRegistrationHandler : public interface::IRegistrationHandler
{
public:
    AdminRegistrationHandler(const std::shared_ptr<interface::IDatabaseController>& adminDatabaseController);
    bool registerPerson(const std::string& login, const std::string& password) override;
    std::map<std::string, std::string> getData() override;
    bool isPersonAlreadyRegistered(const std::map<std::string, std::string>& data, const std::string& login) override;
private:
    std::shared_ptr<interface::IDatabaseController> adminDatabaseController_;
    Logger logger_;
};

} // namespace registration
