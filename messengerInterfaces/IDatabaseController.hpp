#pragma once

#include <map>
#include <string>

namespace interface
{

class IDatabaseController
{
public:
    virtual ~IDatabaseController() {};

    virtual void loadDatabase() = 0;
    virtual void registerPerson(const std::string&, const std::string&) = 0;
    virtual const std::map<std::string, std::string> getData() const = 0;
};

} // namespace interface
