#include <gtest/gtest.h>
#include <iostream>
#include <cstdlib>
#include <filesystem>

#include "src/Database/AdminDatabaseController.hpp"
#include "src/Common/Config/pathConfig.hpp"
#include "common/commonFixture.hpp"

TEST(AdminDatabaseControllerTests, shouldLoadAdminDataFromDatabaseSuccesfully)
{
    Logger::testName_ = "AdminDatabaseControllerTests_shouldLoadDataFromDatabaseSuccesfully";
    const std::string path = config::path::pathPrefix + "libmessenger/tests/testDatabases/databaseControllerDB1/";
    auto adminDatabaseController = std::make_shared<database::AdminDatabaseController>(path);
    adminDatabaseController->loadDatabase();
    EXPECT_EQ(adminDatabaseController->getData().size(), 1);
}

TEST(AdminDatabaseControllerTests, shouldRegisterAdminSavingDataToDatabase)
{
    Logger::testName_ = "AdminDatabaseControllerTests_shouldRegisterUserSavingDataToDatabase";
    const std::string path = config::path::pathPrefix + "libmessenger/tests/testDatabases/databaseControllerDB2/";
    auto adminDatabaseController = std::make_shared<database::AdminDatabaseController>(path);
    auto login = "userLogin123";
    auto password = "userPassword123";

    adminDatabaseController->loadDatabase();
    EXPECT_EQ(adminDatabaseController->getData().size(), 0);

    adminDatabaseController->registerPerson(login, password);
    adminDatabaseController->loadDatabase();
    const auto& data = adminDatabaseController->getData();
    EXPECT_EQ(data.size(), 1);
    EXPECT_EQ(data.begin()->first, "userLogin123");
    EXPECT_EQ(data.begin()->second, "userPassword123");

    clearAllFilesFromFolder(path);
    setDefaultValueForIndexFile(config::path::pathPrefix + "libmessenger/tests/testDatabases/databaseControllerDB2/", "0");
}
