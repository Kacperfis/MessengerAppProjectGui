#pragma once
#include <gmock/gmock.h>
#include "messengerInterfaces/IRegistrationHandler.hpp"

namespace mocks
{

class RegistrationHandlerMock : public interface::IRegistrationHandler
{
public:
    MOCK_METHOD(bool, registrationTrigger, (), (override));
    MOCK_METHOD((std::map<std::string, std::string>), getData, (), (override));
    MOCK_METHOD(bool, isPersonAlreadyRegistered, ((const std::map<std::string, std::string>&)), (override));
    MOCK_METHOD(void, saveDataForLoginAuthentication, (const std::string&, const std::string&), (override));
};

} // namespace mocks
