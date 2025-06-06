#pragma once

#include <memory>
#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "Event.hpp"
#include "Core.hpp"

class ClientGui : public QWidget
{
    Q_OBJECT
public:
    explicit ClientGui(std::shared_ptr<messengerapp::ClientCore> core,
                       QWidget *parent = nullptr);
    ~ClientGui() override;

private slots:
    void onHomeLoginClicked();
    void onHomeRegisterClicked();
    void onLoginClicked();
    void onRegistrationClicked();
    void onSendClicked();

private:
    void buildHomePage();
    void buildLoginPage();
    void buildRegistrationPage();
    void buildChatPage();
    void setChatControlsEnabled(bool enabled);
    void animatedSwitchTo(QWidget *page);
    void handleChatEvent(const ChatEvent &event);

    /* --- members --- */
    std::shared_ptr<messengerapp::ClientCore> clientCore_;
    QStackedWidget *stack_ { nullptr };
    QLabel         *banner_ { nullptr };

    /* home page */
    QWidget     *homePage_ { nullptr };
    QPushButton *homeLoginBtn_ { nullptr };
    QPushButton *homeRegisterBtn_ { nullptr };

    /* login page */
    QWidget   *loginPage_  { nullptr };
    QLineEdit *loginEdit_  { nullptr };
    QLineEdit *passwordEdit_ { nullptr };
    QPushButton *loginBtn_ { nullptr };

    /* registration page */
    QWidget   *registrationPage_  { nullptr };
    QLineEdit *regLoginEdit_ { nullptr };
    QLineEdit *regPasswordEdit_ { nullptr };
    QPushButton *registerBtn_ { nullptr };

    /* chat page */
    QWidget     *chatPage_   { nullptr };
    QTextEdit   *history_    { nullptr };
    QLineEdit   *messageEdit_{ nullptr };
    QPushButton *sendBtn_    { nullptr };
    QGroupBox   *onlineGroup_{ nullptr };
    QListWidget *onlineList_ { nullptr };
};
