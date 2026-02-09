#include <QApplication>
#include "ClientGui.hpp"
#include <Common/Config/pathConfig.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    boost::asio::io_context ioClient;
    std::string userPath = config::path::getUserDatabasePath();
    std::string adminPath = config::path::getAdminDatabasePath();

    auto clientCore = std::make_shared<messengerapp::ClientCore>(ioClient, 1, userPath, adminPath);

    ClientGui gui(clientCore);
    gui.show();

    return app.exec();
}