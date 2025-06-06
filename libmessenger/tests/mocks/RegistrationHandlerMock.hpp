#pragma once

#include <gmock/gmock.h>
#include "messengerInterfaces/IRegistrationHandler.hpp"

namespace mocks
{

class RegistrationHandlerMock : public interface::IRegistrationHandler
{
public:
    MOCK_METHOD(bool, registerPerson,
                (const std::string& login, const std::string& password),
                (override));

    MOCK_METHOD((std::map<std::string, std::string>), getData,
                (),
                (override));

    MOCK_METHOD(bool, isPersonAlreadyRegistered,
                ((const std::map<std::string, std::string>& data),
                 (const std::string& login)),
                (override));
};

} // namespace mocks
