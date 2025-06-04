#pragma once

#include <map>
#include <string>

namespace interface
{

class IRegistrationHandler
{
public:
    virtual ~IRegistrationHandler() {};
    virtual bool registrationTrigger() = 0;
    virtual std::map<std::string, std::string> getData() = 0;
    virtual bool isPersonAlreadyRegistered(const std::map<std::string, std::string>&) = 0;
    virtual void saveDataForLoginAuthentication(const std::string&, const std::string&) = 0;
};

} // namespace interface
