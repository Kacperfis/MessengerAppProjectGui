#include <QApplication>
#include "ServerGui.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    boost::asio::io_context ioServer;
    std::string userPath = "/home/kacper/LibMessengerAppProject/new/libmessenger/src/Database/RegisteredUserData/";
    std::string adminPath = "/home/kacper/LibMessengerAppProject/new/libmessenger/src/Database/RegisteredAdminData/";

    auto serverCore = std::make_shared<messengerapp::ServerCore>(ioServer, 8080);

    ServerGui gui(serverCore);
    gui.show();

    return app.exec();
}