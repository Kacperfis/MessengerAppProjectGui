#include <ranges>
#include "AdminRegistrationHandler.hpp"

namespace registration
{

AdminRegistrationHandler::AdminRegistrationHandler(
    const std::shared_ptr<interface::IDatabaseController>& adminDatabaseController) :
    adminDatabaseController_(adminDatabaseController),
    logger_("AdminRegistrationHandler") {}


std::map<std::string, std::string> AdminRegistrationHandler::getData()
{
    adminDatabaseController_->loadDatabase();
    return adminDatabaseController_->getData();
}

bool AdminRegistrationHandler::registerPerson(const std::string& login, const std::string& password)
{
    auto data = getData();
    if (isPersonAlreadyRegistered(data, login))
    {
        return false;
    }

    adminDatabaseController_->registerPerson(login, password);
    logger_.log(Severity::info, "Admin registered succesfully");
    return true;
}

bool AdminRegistrationHandler::isPersonAlreadyRegistered(const std::map<std::string, std::string>& data, const std::string& login)
{
    auto adminData = std::ranges::find_if(data,
    [&login](const auto& item)
    {
        return item.first == login;
    });

    if (adminData != data.end())
    {
        logger_.log(Severity::warning, "Admin with given login is already registered");
        return true;
    }

    return false;
}

} // namespace registration
