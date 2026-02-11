#include <QApplication>
#include "ClientGui.hpp"
#include <Common/Config/pathConfig.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int clientId = 1;
    if (argc > 1)
    {
        try {
            clientId = std::stoi(argv[1]);
        } catch (...) {
            std::cerr << "Invalid client ID argument, using default (1)" << std::endl;
            clientId = 1;
        }
    }

    boost::asio::io_context ioClient;
    std::string userPath = config::path::getUserDatabasePath();
    std::string adminPath = config::path::getAdminDatabasePath();

    auto clientCore = std::make_shared<messengerapp::ClientCore>(ioClient, clientId, userPath, adminPath);

    ClientGui gui(clientCore);
    gui.show();

    return app.exec();
}