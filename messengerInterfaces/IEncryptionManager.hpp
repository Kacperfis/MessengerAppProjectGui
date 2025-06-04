#pragma once
#include <string>
#include <vector>

namespace interface
{

class IEncryptionManager
{
public:
    virtual ~IEncryptionManager() = default;
    virtual void encryptDataAndSaveToDatabase(const std::string& login, const std::string& password) = 0;
    virtual std::vector<std::string> decryptDataFromDatabase() = 0;
    virtual std::string encryptString(const std::string& plaintext) const = 0;
    virtual std::string decryptString(const std::string& ciphertext) const = 0;
};

} // namespace encryption
