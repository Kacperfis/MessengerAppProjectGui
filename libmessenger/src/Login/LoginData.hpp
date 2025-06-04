#pragma once
#include <string>

namespace login
{

struct LoginData
{
public:
    LoginData(const std::string& login, const std::string& password) : login_(login), password_(password){}

    const std::string getLogin() { return login_; }
    const std::string getPassword(){ return password_; }

protected:
    std::string login_;
    std::string password_;
};

enum class loginStatus
{
    Logged,
    LoggedOut
};

} // namespace login
