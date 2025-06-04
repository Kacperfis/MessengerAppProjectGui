#pragma once
#include <memory>
#include <string>
#include <map>
#include "Common/Logger.hpp"
#include "Encryption/EncryptionManager.hpp"
#include "messengerInterfaces/IDatabaseController.hpp"

namespace database
{

class AdminDatabaseController : public interface::IDatabaseController
{
public:
    explicit AdminDatabaseController(const std::string& usersDatabasePath);
    void loadDatabase() override;
    void registerPerson(const std::string& login, const std::string& password) override;
    const std::map<std::string, std::string> getData() const override;
private:
    const std::unique_ptr<encryption::EncryptionManager> adminEncryptionManager_;

    std::map<std::string, std::string> registeredAdminData_;
    Logger logger_;
};

} // namespace database
