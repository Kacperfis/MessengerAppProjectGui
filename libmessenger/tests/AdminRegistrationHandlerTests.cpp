// #include <gtest/gtest.h>
// #include <iostream>

// #include "common/commonFixture.hpp"
// #include "src/Registration/AdminRegistrationHandler.hpp"
// #include "mocks/DatabaseControllerMock.hpp"

// using testing::Return;
// using testing::_;

// TEST(AdminRegistrationHandlerTests, shouldTriggerRegistrationAndRegisterAdminSuccessfully)
// {
//     Logger::testName_ = "AdminRegistrationHandlerTests_shouldTriggerRegistrationAndRegisterUserSuccessfully";

//     auto databaseControllerMock = std::make_shared<mocks::DatabaseControllerMock>();
//     auto sut = std::make_shared<registration::AdminRegistrationHandler>(databaseControllerMock);
//     std::map<std::string, std::string> registeredUsersData;
//     registeredUsersData.insert(std::make_pair("test_admin", "test_password"));

//     EXPECT_CALL(*databaseControllerMock, loadDatabase())
//         .Times(1);

//     EXPECT_CALL(*databaseControllerMock, getData())
//         .WillOnce(Return(registeredUsersData));

//     EXPECT_CALL(*databaseControllerMock, registerPerson(_,_))
//         .Times(1);

//     auto result = sut->registrationTrigger();
//     EXPECT_TRUE(result);
// }

// TEST(AdminRegistrationHandlerTests, shouldTriggerRegistrationAndNotRegisterAlreadyRegisteredAdmin)
// {
//     Logger::testName_ = "AdminRegistrationHandlerTests_shouldTriggerRegistrationAndNotRegisterAlreadyRegisteredUser";

//     auto databaseControllerMock = std::make_shared<mocks::DatabaseControllerMock>();
//     auto sut = std::make_shared<registration::AdminRegistrationHandler>(databaseControllerMock);
//     std::map<std::string, std::string> registeredUsersData;
//     registeredUsersData.insert(std::make_pair("test_admin", "test_password"));

//     EXPECT_CALL(*databaseControllerMock, loadDatabase())
//         .Times(1);

//     EXPECT_CALL(*databaseControllerMock, getData())
//         .WillOnce(Return(registeredUsersData));

//     sut->saveDataForLoginAuthentication("test_admin", "test_password");
//     auto result = sut->registrationTrigger();
//     EXPECT_FALSE(result);
// }
