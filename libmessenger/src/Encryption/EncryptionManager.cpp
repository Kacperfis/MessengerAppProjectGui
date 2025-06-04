#include "EncryptionManager.hpp"
#include <openssl/evp.h>

namespace encryption
{

EncryptionManager::EncryptionManager(const std::string& path, const std::string& key, const std::string& iv)
    : path_(path), key_(key), iv_(iv), logger_("EncryptionManager") {}

void EncryptionManager::encryptDataAndSaveToDatabase(const std::string& login, const std::string& password)
{
    auto plaintext = login + "|" + password;
    auto ciphertext = encryptString(plaintext);
    auto currentFileIndex = getCurrentFileIndex(path_);
    if (currentFileIndex.empty())
    {
        logger_.log(Severity::warning, "empty current file index, unable to encrypt");
        return;
    }
    auto currentIndex = std::stoi(currentFileIndex) + 1;
    std::ofstream file(path_ + "data" + std::to_string(currentIndex) + ".txt", std::ios::binary | std::ios::app);

    if (file.is_open())
    {
        file.write(ciphertext.data(), ciphertext.size());
        if (file.fail() || file.bad()) logger_.log(Severity::error, "Error writing to file");
        file.close();
        updateCurrentFileIndex(path_, currentIndex);
        logger_.log(Severity::info, "Encrypted data succesfully saved to users database");
    }
    else logger_.log(Severity::error, "Error opening file");
}

std::vector<std::string> EncryptionManager::decryptDataFromDatabase()
{
    std::vector<std::string> decryptedContent;
    auto currentFileIndex = getCurrentFileIndex(path_);
    if (currentFileIndex.empty())
    {
        logger_.log(Severity::warning, "empty current file index, unable to decrypt");
        return {};
    }

    auto currentNumberOfFiles = std::stoi(currentFileIndex);
    for (int counter = 1; counter <= currentNumberOfFiles; counter++)
    {
        std::ifstream infile(path_ + "data" + std::to_string(counter) + ".txt", std::ios::in | std::ios::binary);
        if (!infile.is_open())
        {
            logger_.log(Severity::error, "Failed to open file: " + path_ + "data" + std::to_string(counter));
            return {};
        }

        infile.seekg(0, std::ios::end);
        std::streamsize size = infile.tellg();
        infile.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (!infile.read(buffer.data(), size))
        {
            logger_.log(Severity::error, "Error reading from the file");
            return {};
        }

        std::string ciphertext(buffer.begin(), buffer.end());
        decryptedContent.push_back(decryptString(ciphertext));

        infile.close();
    }

    logger_.log(Severity::info, "Successfully decrypted data from users database");
    return decryptedContent;
}

std::string EncryptionManager::getCurrentFileIndex(const std::string& path)
{
    std::string index = "";
    std::ifstream indexFile(path + "dataIndex.txt");
    if (indexFile.is_open())
    {
        if (getline(indexFile, index)) logger_.log(Severity::info, "successfully read current index: " + index);
        else logger_.log(Severity::error, "cannot read current index from file");
    }
    else logger_.log(Severity::error, "Error opening index file");
    indexFile.close();

    return index;
}

void EncryptionManager::updateCurrentFileIndex(const std::string& path, const int& currentIndex)
{
    std::ofstream indexFile(path + "dataIndex.txt", std::ios::trunc);

    if (indexFile.is_open())
    {
        indexFile << currentIndex;
        logger_.log(Severity::info, "Successfully updated current Index of file");
        indexFile.close();
    }
    else logger_.log(Severity::error, "Error opening index file to update");
}

std::string EncryptionManager::encryptString(const std::string& plaintext) const
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    auto charTypeKey = reinterpret_cast<const unsigned char*>(key_.c_str());
    auto charTypeIv = reinterpret_cast<const unsigned char*>(iv_.c_str());
    auto charTypePlaintext = reinterpret_cast<const unsigned char*>(plaintext.c_str());

    auto ciphertext = std::make_unique<unsigned char[]>(plaintext.length() + EVP_MAX_BLOCK_LENGTH);
    auto length = std::make_unique<int>(), finalLength = std::make_unique<int>();

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, charTypeKey, charTypeIv);
    EVP_EncryptUpdate(ctx, ciphertext.get(), length.get(), charTypePlaintext, plaintext.length());
    EVP_EncryptFinal_ex(ctx, ciphertext.get() + *length, finalLength.get());
    EVP_CIPHER_CTX_free(ctx);

    return std::string(reinterpret_cast<char*>(ciphertext.get()), *length + *finalLength);
}

std::string EncryptionManager::decryptString(const std::string& ciphertext) const
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    auto charTypeKey = reinterpret_cast<const unsigned char*>(key_.c_str());
    auto charTypeIv = reinterpret_cast<const unsigned char*>(iv_.c_str());
    auto charTypeCiphertext = reinterpret_cast<const unsigned char*>(ciphertext.data());

    auto plaintext = std::make_unique<unsigned char[]>(ciphertext.length() + EVP_MAX_BLOCK_LENGTH);
    auto lentgh = std::make_unique<int>(), finalLength = std::make_unique<int>();

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, charTypeKey, charTypeIv);
    EVP_DecryptUpdate(ctx, plaintext.get(), lentgh.get(), charTypeCiphertext, ciphertext.length());
    EVP_DecryptFinal_ex(ctx, plaintext.get() + *lentgh, finalLength.get());
    EVP_CIPHER_CTX_free(ctx);

    return std::string(reinterpret_cast<char*>(plaintext.get()), *lentgh + *finalLength);
}

} // namespace encryption
