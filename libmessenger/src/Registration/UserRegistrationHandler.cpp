#include <ranges>
#include "UserRegistrationHandler.hpp"

namespace registration
{

UserRegistrationHandler::UserRegistrationHandler(
    const std::shared_ptr<interface::IDatabaseController>& userDatabaseController) :
    userDatabaseController_(userDatabaseController),
    logger_("UserRegistrationHandler") {}

std::map<std::string, std::string> UserRegistrationHandler::getData()
{
    userDatabaseController_->loadDatabase();
    return userDatabaseController_->getData();
}

bool UserRegistrationHandler::registerPerson(const std::string& login, const std::string& password)
{
    auto data = getData();
    if (isPersonAlreadyRegistered(data, login))
    {
        return false;
    }

    userDatabaseController_->registerPerson(login, password);
    logger_.log(Severity::info, "User registered succesfully");
    return true;
}

bool UserRegistrationHandler::isPersonAlreadyRegistered(const std::map<std::string, std::string>& data, const std::string& login)
{
    auto userData = std::ranges::find_if(data,
    [&login](const auto& item)
    {
        return item.first == login;
    });

    if (userData != data.end())
    {
        logger_.log(Severity::warning, "User with given login is already registered");
        return true;
    }

    return false;
}

} // namespace registration
