#include "AdminDatabaseController.hpp"
#include <vector>
#include <ranges>

namespace database
{

AdminDatabaseController::AdminDatabaseController(const std::string& adminDatabasePath) :
    adminEncryptionManager_(std::make_unique<encryption::EncryptionManager>(adminDatabasePath, "0123456789abcdef0123456789abcdef", "0123456789abcdef")),
    logger_("AdminDatabaseController") {}

void AdminDatabaseController::loadDatabase()
{
    const auto adminData = adminEncryptionManager_->decryptDataFromDatabase();
    for(const auto& word : adminData)
    {
        for (uint32_t i = 0; i < word.length(); i++)
        {
            if (word[i] == '|')
            {
                registeredAdminData_.insert(std::make_pair<std::string, std::string>(word.substr(0, i), word.substr(i+1, word.length()-1)));
                break;
            }
        }
    }
    logger_.log(Severity::info, "admin database loaded succesfully");
}

void AdminDatabaseController::registerPerson(const std::string& login, const std::string& password)
{
    adminEncryptionManager_->encryptDataAndSaveToDatabase(login, password);
}

const std::map<std::string, std::string> AdminDatabaseController::getData() const
{
    return registeredAdminData_;
}

} // namespace database
