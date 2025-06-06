#pragma once

#include <map>
#include <string>

namespace interface
{

class IRegistrationHandler
{
public:
    virtual ~IRegistrationHandler() {};
    virtual bool registerPerson(const std::string& login, const std::string& password) = 0;
    virtual std::map<std::string, std::string> getData() = 0;
    virtual bool isPersonAlreadyRegistered(const std::map<std::string, std::string>& data, const std::string& login) = 0;
};

} // namespace interface
