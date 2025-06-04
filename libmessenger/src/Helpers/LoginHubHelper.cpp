#include <iostream>
#include <algorithm>
#include <ranges>
#include <map>
#include "LoginHubHelper.hpp"

namespace helpers
{

bool checkUserCredentials(const std::string& login, const std::string& password, const std::map<std::string, std::string>& registeredUsersData)
{
    auto userData = std::ranges::find_if(registeredUsersData, [&login](const auto& item){
        return item.first == login;
    });
    if (userData != registeredUsersData.end()) return userData->second == password;
    return false;
}

}  // namespace helpers
