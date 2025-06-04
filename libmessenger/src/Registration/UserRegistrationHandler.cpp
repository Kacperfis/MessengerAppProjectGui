#include <ranges>
#include "UserRegistrationHandler.hpp"

namespace registration
{

UserRegistrationHandler::UserRegistrationHandler(
    const std::shared_ptr<interface::IDatabaseController>& userDatabaseController) :
    userDatabaseController_(userDatabaseController),
    logger_("UserRegistrationHandler") {}

bool UserRegistrationHandler::registrationTrigger()
{
    return registerUser();
}

std::map<std::string, std::string> UserRegistrationHandler::getData()
{
    userDatabaseController_->loadDatabase();
    return userDatabaseController_->getData();
}

bool UserRegistrationHandler::registerUser()
{
    auto data = getData();
    if (!isPersonAlreadyRegistered(data))
    {
        userDatabaseController_->registerPerson(login_, password_);
        logger_.log(Severity::info, "User registered succesfully");
        return true;
    }
    return false;
}

bool UserRegistrationHandler::isPersonAlreadyRegistered(const std::map<std::string, std::string>& data)
{
    auto userData = std::ranges::find_if(data,
    [this](const auto& item)
    {
        return item.first == login_;
    });

    if (userData != data.end())
    {
        logger_.log(Severity::warning, "User with given login is already registered");
        return true;
    }

    return false;
}

void UserRegistrationHandler::saveDataForLoginAuthentication(const std::string& login, const std::string& password)
{
    login_ = login;
    password_ = password;
}

} // namespace registration
