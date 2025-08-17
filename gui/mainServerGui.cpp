#include <QApplication>
#include "ServerGui.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    boost::asio::io_context ioServer;
    std::string adminPath = "/home/kacper/LibMessengerAppProject/new/libmessenger/src/Database/RegisteredAdminData/";

    ServerGui gui(ioServer, adminPath);
    gui.show();

    return app.exec();
}