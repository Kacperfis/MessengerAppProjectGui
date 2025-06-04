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
    bool registrationTrigger() override;
    std::map<std::string, std::string> getData() override;
    bool isPersonAlreadyRegistered(const std::map<std::string, std::string>& data) override;
    void saveDataForLoginAuthentication(const std::string& login, const std::string& password) override;
private:
    bool registerUser();

    std::string login_;
    std::string password_;

    std::shared_ptr<interface::IDatabaseController> userDatabaseController_;
    Logger logger_;
};

} // namespace registration
