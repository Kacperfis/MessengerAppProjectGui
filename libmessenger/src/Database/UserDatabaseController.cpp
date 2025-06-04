#include "UserDatabaseController.hpp"
#include <vector>
#include <ranges>

namespace database
{

UserDatabaseController::UserDatabaseController(const std::string& usersDatabasePath) :
    userEncryptionManager_(std::make_unique<encryption::EncryptionManager>(usersDatabasePath, "0123456789abcdef0123456789abcdef", "0123456789abcdef")),
    logger_("UserDatabaseController") {}

void UserDatabaseController::loadDatabase()
{
    const auto usersData = userEncryptionManager_->decryptDataFromDatabase();
    for(const auto& word : usersData)
    {
        for (uint32_t i = 0; i < word.length(); i++)
        {
            if (word[i] == '|')
            {
                registeredUsersData_.insert(std::make_pair<std::string, std::string>(word.substr(0, i), word.substr(i+1, word.length()-1)));
                break;
            }
        }
    }
    logger_.log(Severity::info, "user database loaded succesfully");
}

void UserDatabaseController::registerPerson(const std::string& login, const std::string& password)
{
    userEncryptionManager_->encryptDataAndSaveToDatabase(login, password);
}

const std::map<std::string, std::string> UserDatabaseController::getData() const
{
    return registeredUsersData_;
}

} // namespace database
