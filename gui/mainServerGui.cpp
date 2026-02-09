#include <QApplication>
#include "ServerGui.hpp"
#include <Common/Config/pathConfig.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    std::string adminPath = config::path::getAdminDatabasePath();

    ServerGui gui(adminPath);
    gui.show();

    return app.exec();
}