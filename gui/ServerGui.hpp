#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>

#include <boost/asio/io_context.hpp>
#include "ServerCore.hpp"

class ServerGui : public QWidget
{
    Q_OBJECT
public:
    explicit ServerGui(
        std::shared_ptr<messengerapp::ServerCore> serverCore,
        QWidget *parent = nullptr);
    ~ServerGui();

private:
    QPushButton *startServerBtn_;
    std::shared_ptr<messengerapp::ServerCore> serverCore_;
};
