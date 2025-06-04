#pragma once

#include <memory>

#include "messengerInterfaces/ILoginHub.hpp"
#include "messengerInterfaces/IRegistrationHandler.hpp"

#include "LoginData.hpp"
#include "Common/Logger.hpp"

namespace login
{

class AdminLoginHub : public interface::ILoginHub
{
public:
    AdminLoginHub();
    bool login(const std::shared_ptr<LoginData>& loginData, const std::shared_ptr<interface::IRegistrationHandler>& registrationHandler) override;
    bool logout() override;
    bool setStatus(login::loginStatus) override;
    bool isLogged() override;
    const std::string getUserLogin() override;
private:
    bool adminLogged_;
    std::shared_ptr<login::LoginData> loginData_;
    Logger logger_;
};

} // namespace login
