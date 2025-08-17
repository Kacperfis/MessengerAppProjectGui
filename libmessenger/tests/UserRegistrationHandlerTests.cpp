#include <gtest/gtest.h>
#include <iostream>

#include "common/commonFixture.hpp"
#include "src/Registration/UserRegistrationHandler.hpp"
#include "mocks/DatabaseControllerMock.hpp"

using testing::Return;
using testing::_;

TEST(UserRegistrationHandlerTests, shouldTriggerRegistrationAndRegisterUserSuccessfully)
{
    Logger::testName_ = "UserRegistrationHandlerTests_shouldTriggerRegistrationAndRegisterUserSuccessfully";

    auto databaseControllerMock = std::make_shared<mocks::DatabaseControllerMock>();
    auto sut = std::make_shared<registration::UserRegistrationHandler>(databaseControllerMock);
    std::map<std::string, std::string> registeredUsersData;
    registeredUsersData.insert(std::make_pair("test_user2", "test_password2"));

    EXPECT_CALL(*databaseControllerMock, loadDatabase())
        .Times(1);

    EXPECT_CALL(*databaseControllerMock, getData())
        .WillOnce(Return(registeredUsersData));

    EXPECT_CALL(*databaseControllerMock, registerPerson(_,_))
        .Times(1);

    auto result = sut->registerPerson("test_user", "test_password");
    EXPECT_TRUE(result);
}

TEST(UserRegistrationHandlerTests, shouldTriggerRegistrationAndNotRegisterAlreadyRegisteredUser)
{
    Logger::testName_ = "UserRegistrationHandlerTests_shouldTriggerRegistrationAndNotRegisterAlreadyRegisteredUser";

    auto databaseControllerMock = std::make_shared<mocks::DatabaseControllerMock>();
    auto sut = std::make_shared<registration::UserRegistrationHandler>(databaseControllerMock);
    std::map<std::string, std::string> registeredUsersData;
    registeredUsersData.insert(std::make_pair("test_user", "test_password"));

    EXPECT_CALL(*databaseControllerMock, loadDatabase())
        .Times(1);

    EXPECT_CALL(*databaseControllerMock, getData())
        .WillOnce(Return(registeredUsersData));

    auto result = sut->registerPerson("test_user", "test_password");
    EXPECT_FALSE(result);
}
