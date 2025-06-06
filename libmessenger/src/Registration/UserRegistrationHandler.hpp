#pragma once
#include <memory>
#include <string>
#include <iostream>

#include "Database/UserDatabaseController.hpp"
#include "messengerInterfaces/IRegistrationHandler.hpp"

namespace registration
{

class UserRegistrationHandler : public interface::IRegistrationHandler
{
public:
    UserRegistrationHandler(const std::shared_ptr<interface::IDatabaseController>& userDatabaseController);
    bool registerPerson(const std::string& login, const std::string& password) override;
    std::map<std::string, std::string> getData() override;
    bool isPersonAlreadyRegistered(const std::map<std::string, std::string>& data, const std::string& login) override;
private:
    std::shared_ptr<interface::IDatabaseController> userDatabaseController_;
    Logger logger_;
};

} // namespace registration
