#include <gtest/gtest.h>
#include <iostream>

#include "src/Login/UserLoginHubCreator.hpp"
#include "src/Login/UserLoginHub.hpp"
#include "src/Common/Logger.hpp"

#include "common/commonFixture.hpp"
#include "mocks/RegistrationHandlerMock.hpp"

using testing::Return;
using testing::_;

TEST(UserLoginHubTests, shouldLoginAndLogoutSuccesfully)
{
    Logger::testName_ = "UserLoginHubTests_ShouldLoginAndLogoutSuccesfully";

    auto registrationHandlerMock = std::make_shared<mocks::RegistrationHandlerMock>();
    auto LoginHubCreator = std::make_shared<login::UserLoginHubCreator>();
    auto LoginHub = LoginHubCreator->createHub();

    auto loginData = std::make_shared<login::LoginData>("test_user", "test_password");
    std::map<std::string, std::string> registeredUsersData;
    registeredUsersData.insert(std::make_pair("test_user", "test_password"));

    EXPECT_CALL(*registrationHandlerMock, getData())
        .WillOnce(Return(registeredUsersData));


    EXPECT_CALL(*registrationHandlerMock, isPersonAlreadyRegistered(registeredUsersData, loginData->getLogin()))
        .WillOnce(Return(true));

    EXPECT_TRUE(LoginHub->login(loginData, registrationHandlerMock));
    EXPECT_TRUE(LoginHub->isLogged());
    EXPECT_TRUE(LoginHub->logout());
    EXPECT_FALSE(LoginHub->isLogged());
}

TEST(UserLoginHubTests, shouldNotLoginDueToNotExistingUserDataInDatabase)
{
    Logger::testName_ = "UserLoginHubTests_ShouldNotLoginDueToNotExistingUserDataInDatabase";

    auto registrationHandlerMock = std::make_shared<mocks::RegistrationHandlerMock>();
    auto LoginHubCreator = std::make_shared<login::UserLoginHubCreator>();
    auto LoginHub = LoginHubCreator->createHub();

    auto loginData = std::make_shared<login::LoginData>("test_user2", "test_password2");
    std::map<std::string, std::string> registeredUsersData;
    registeredUsersData.insert(std::make_pair("test_user", "test_password"));

    EXPECT_CALL(*registrationHandlerMock, getData())
        .WillOnce(Return(registeredUsersData));

    EXPECT_CALL(*registrationHandlerMock, isPersonAlreadyRegistered(registeredUsersData, loginData->getLogin()))
        .WillOnce(Return(false));

    EXPECT_FALSE(LoginHub->login(loginData, registrationHandlerMock));
    EXPECT_FALSE(LoginHub->isLogged());
}
