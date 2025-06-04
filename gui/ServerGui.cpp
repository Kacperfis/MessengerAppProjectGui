#include "ServerGui.hpp"
#include <QVBoxLayout>

ServerGui::ServerGui(
    std::shared_ptr<messengerapp::ServerCore> serverCore,
    QWidget *parent)
    : serverCore_(serverCore),
      QWidget(parent)
{
    startServerBtn_ = new QPushButton("start Server", this);

    auto *lay = new QVBoxLayout(this);
    lay->addWidget(startServerBtn_);
    setLayout(lay);
    resize(200, 200);

    serverCore_->startServer();
}

ServerGui::~ServerGui()
{
    serverCore_->stopServer();
}
