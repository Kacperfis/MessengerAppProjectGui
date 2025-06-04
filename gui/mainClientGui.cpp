#include <QApplication>
#include "ClientGui.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    boost::asio::io_context ioClient;
    std::string userPath = "/home/kacper/LibMessengerAppProject/new/libmessenger/src/Database/RegisteredUserData/";
    std::string adminPath = "/home/kacper/LibMessengerAppProject/new/libmessenger/src/Database/RegisteredAdminData/";

    auto clientCore = std::make_shared<messengerapp::ClientCore>(ioClient, 1, userPath, adminPath);

    ClientGui gui(clientCore);
    gui.show();

    return app.exec();
}