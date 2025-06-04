#pragma once
#include <gmock/gmock.h>
#include "messengerInterfaces/IEncryptionManager.hpp"

namespace mocks
{

class EncryptionManagerMock : public interface::IEncryptionManager
{
public:
    MOCK_METHOD(std::string, decryptString, (const std::string&), (const, override));
    MOCK_METHOD(std::string, encryptString, (const std::string&), (const, override));
    MOCK_METHOD(void, encryptDataAndSaveToDatabase, (const std::string&, const std::string&), (override));
    MOCK_METHOD(std::vector<std::string>, decryptDataFromDatabase, (), (override));
};

} // namespace mocks
