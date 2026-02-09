#ifndef SERVERGUI_HPP
#define SERVERGUI_HPP

#include <QWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>

#include <memory>
#include <string>
#include <thread>
#include <optional>

#include "Core.hpp"

class ServerGui : public QWidget {
    Q_OBJECT

public:
    ServerGui(const std::string& adminDatabasePath, QWidget* parent = nullptr);
    ~ServerGui();

private slots:
    void onHomeLoginClicked();
    void onHomeRegisterClicked();
    void onLoginClicked();
    void onRegistrationClicked();
    void onStartServerClicked();
    void onStopServerClicked();

private:
    void buildHomePage();
    void buildLoginPage();
    void buildRegistrationPage();
    void buildMainPage();
    void animatedSwitchTo(QWidget *page);

    std::shared_ptr<messengerapp::ServerCore> serverCore_;

    /* --- members --- */
    QLabel* banner_;
    QStackedWidget* stack_;
    QWidget* homePage_;
    QWidget* loginPage_;
    QWidget* registrationPage_;
    QWidget* mainPage_;

    /* home page */
    QPushButton* homeLoginBtn_;
    QPushButton* homeRegisterBtn_;

    /* login page */
    QLineEdit* loginEdit_;
    QLineEdit* passwordEdit_;
    QPushButton* loginBtn_;

    /* registration page */
    QLineEdit* regLoginEdit_;
    QLineEdit* regPasswordEdit_;
    QPushButton* registerBtn_;
};

#endif // SERVERGUI_HPP