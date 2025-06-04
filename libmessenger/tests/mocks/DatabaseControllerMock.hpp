#pragma once
#include <gmock/gmock.h>
#include "messengerInterfaces/IDatabaseController.hpp"

namespace mocks
{

class DatabaseControllerMock : public interface::IDatabaseController
{
public:
    MOCK_METHOD(void, loadDatabase, (), (override));
    MOCK_METHOD(void, registerPerson, (const std::string&, const std::string&), (override));
    MOCK_METHOD((const std::map<std::string, std::string>), getData, (), (const override));
};

} // namespace mocks
