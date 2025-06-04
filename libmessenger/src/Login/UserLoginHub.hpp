#pragma once
#include <memory>
#include <iostream>

#include "messengerInterfaces/ILoginHub.hpp"
#include "messengerInterfaces/IRegistrationHandler.hpp"

#include "Login/LoginData.hpp"
#include "Common/Logger.hpp"

namespace login
{

class UserLoginHub : public interface::ILoginHub
{
public:
    UserLoginHub();
    bool login(const std::shared_ptr<LoginData>& loginData, const std::shared_ptr<interface::IRegistrationHandler>& registrationHandler) override;
    bool logout() override;
    bool setStatus(login::loginStatus) override;
    bool isLogged() override;
    const std::string getUserLogin() override;
private:
    bool userLogged_;
    std::shared_ptr<login::LoginData> loginData_;
    Logger logger_;
};

} // namespace login
