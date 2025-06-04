#include <iostream>

#include "AdminLoginHub.hpp"
#include "Helpers/LoginHubHelper.hpp"

namespace login
{

AdminLoginHub::AdminLoginHub() : logger_("AdminLoginHub") {}

bool AdminLoginHub::login(const std::shared_ptr<LoginData>& loginData, const std::shared_ptr<interface::IRegistrationHandler>& registrationHandler)
{
    logger_.log(Severity::info, "Logging to the Admin account");
    loginData_ = loginData;
    auto registeredAdminData = registrationHandler->getData();
    registrationHandler->saveDataForLoginAuthentication(loginData_->getLogin(), loginData_->getPassword());

    if (registrationHandler->isPersonAlreadyRegistered(registeredAdminData))
    {
        if (helpers::checkUserCredentials(loginData_->getLogin(), loginData_->getPassword(), registeredAdminData))
        {
            if (!setStatus(login::loginStatus::Logged))
            {
                logger_.log(Severity::warning, "Login unsuccessful");
                return false;
            }
            logger_.log(Severity::info, "Login successful");
            return true;
        }
    }
    return false;
}

bool AdminLoginHub::logout()
{
    if (!setStatus(login::loginStatus::LoggedOut))
    {
        logger_.log(Severity::warning, "Logout unsuccessful");
        return false;
    }
    return true;
}

bool AdminLoginHub::isLogged()
{
    return adminLogged_;
}

bool AdminLoginHub::setStatus(login::loginStatus status)
{
    switch (status)
    {
        case login::loginStatus::Logged:
            adminLogged_ = true;
            break;
        case login::loginStatus::LoggedOut:
            adminLogged_ = false;
            break;
    }
    return true;
}

const std::string AdminLoginHub::getUserLogin()
{
    return loginData_->getLogin();
}

} // namespace login
