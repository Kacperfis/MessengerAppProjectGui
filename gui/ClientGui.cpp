#include "ClientGui.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QMetaObject>

ClientGui::ClientGui(std::shared_ptr<messengerapp::ClientCore> core,
                     QWidget *parent)
    : QWidget(parent),
      clientCore_(std::move(core))
{
    /* ----- basic window setup ----- */
    setWindowTitle("MessengerApp");
    resize(600, 700);

    /* dark‑mode / modern stylesheet  */
    setStyleSheet(R"(
        QWidget {
            background-color: #1e1e1e;
            color: #e0e0e0;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
            font-size: 15px;
        }
        QPushButton {
            background-color: #2d2d30;
            border: 1px solid #3e3e42;
            border-radius: 6px;
            padding: 6px 12px;
        }
        QPushButton:hover {
            background-color: #3e3e42;
        }
        QPushButton:pressed {
            background-color: #0078d7;
        }
        QLineEdit, QTextEdit, QListWidget {
            background-color: #252526;
            border: 1px solid #3e3e42;
            border-radius: 4px;
            selection-background-color: #0078d7;
        }
        QGroupBox {
            border: 1px solid #3e3e42;
            border-radius: 6px;
            margin-top: 25px;    /* space for title */
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 3px 0 3px;
            color: #bbbbbb;
            font-weight: 600;
        }
    )");

    banner_ = new QLabel("MessengerApp", this);
    banner_->setAlignment(Qt::AlignCenter);
    banner_->setStyleSheet("font-size: 32px; font-weight: 600; color: #ffffff; "
                           "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0078d7, stop:1 #00d7a7);"
                           "padding: 18px;");

    stack_ = new QStackedWidget(this);
    buildHomePage();
    buildLoginPage();
    buildRegistrationPage();
    buildChatPage();

    auto *root = new QVBoxLayout(this);
    root->setSpacing(0);
    root->setContentsMargins(0,0,0,0);
    root->addWidget(banner_);
    root->addWidget(stack_);
    setLayout(root);

    stack_->setCurrentWidget(homePage_);
    setChatControlsEnabled(false);

    clientCore_->setEventHandler([this](const ChatEvent &ev) {
        QMetaObject::invokeMethod(
            this,
            [this, ev] { handleChatEvent(ev); },
            Qt::QueuedConnection);
    });

    clientCore_->startClient("127.0.0.1", "8080");
}

ClientGui::~ClientGui()
{
    clientCore_->stopClient();
}

void ClientGui::buildHomePage()
{
    homePage_ = new QWidget(this);
    auto *v = new QVBoxLayout(homePage_);

    homeLoginBtn_    = new QPushButton("Log in", homePage_);
    homeRegisterBtn_ = new QPushButton("Register", homePage_);

    v->addStretch(1);
    v->addWidget(homeLoginBtn_);
    v->addWidget(homeRegisterBtn_);
    v->addStretch(2);

    connect(homeLoginBtn_, &QPushButton::clicked,
        this, &ClientGui::onHomeLoginClicked);

    connect(homeRegisterBtn_, &QPushButton::clicked,
        this, &ClientGui::onHomeRegisterClicked);

    stack_->addWidget(homePage_);
}

void ClientGui::buildLoginPage()
{
    loginPage_ = new QWidget(this);
    auto *form = new QFormLayout(loginPage_);

    loginEdit_    = new QLineEdit(loginPage_);
    passwordEdit_ = new QLineEdit(loginPage_);
    passwordEdit_->setEchoMode(QLineEdit::Password);
    loginBtn_     = new QPushButton("Log in", loginPage_);

    form->addRow("Login:",    loginEdit_);
    form->addRow("Password:", passwordEdit_);
    form->addRow(loginBtn_);

    connect(loginBtn_, &QPushButton::clicked,
            this, &ClientGui::onLoginClicked);

    stack_->addWidget(loginPage_);
}

void ClientGui::buildRegistrationPage()
{
    registrationPage_ = new QWidget(this);
    auto *form = new QFormLayout(registrationPage_);

    regLoginEdit_ = new QLineEdit(registrationPage_);
    regPasswordEdit_ = new QLineEdit(registrationPage_);
    regPasswordEdit_->setEchoMode(QLineEdit::Password);
    registerBtn_ = new QPushButton("Register", registrationPage_);

    form->addRow("New Login:", regLoginEdit_);
    form->addRow("New Password:", regPasswordEdit_);
    form->addRow(registerBtn_);

    connect(registerBtn_, &QPushButton::clicked,
        this, &ClientGui::onRegistrationClicked);

    stack_->addWidget(registrationPage_);
}

void ClientGui::buildChatPage()
{
    chatPage_ = new QWidget(this);

    history_ = new QTextEdit(chatPage_);
    history_->setReadOnly(true);

    messageEdit_ = new QLineEdit(chatPage_);
    sendBtn_     = new QPushButton("Send", chatPage_);

    auto *msgRow = new QHBoxLayout;
    msgRow->addWidget(messageEdit_);
    msgRow->addWidget(sendBtn_);

    auto *leftCol = new QVBoxLayout;
    leftCol->addWidget(history_);
    leftCol->addLayout(msgRow);

    onlineGroup_ = new QGroupBox("Active Users", chatPage_);
    onlineList_  = new QListWidget(onlineGroup_);
    auto *onlineLayout = new QVBoxLayout(onlineGroup_);
    onlineLayout->addWidget(onlineList_);
    onlineGroup_->setFixedWidth(180);

    auto *mainRow = new QHBoxLayout(chatPage_);
    mainRow->addLayout(leftCol, 3);
    mainRow->addWidget(onlineGroup_, 1);

    connect(sendBtn_,  &QPushButton::clicked,
            this,      &ClientGui::onSendClicked);

    stack_->addWidget(chatPage_);
}

void ClientGui::onHomeLoginClicked()
{
    loginEdit_->clear();
    passwordEdit_->clear();
    animatedSwitchTo(loginPage_);
}

void ClientGui::onHomeRegisterClicked()
{
    regLoginEdit_->clear();
    regPasswordEdit_->clear();
    animatedSwitchTo(registrationPage_);
}

void ClientGui::onLoginClicked()
{
    const std::string login    = loginEdit_->text().toStdString();
    const std::string password = passwordEdit_->text().toStdString();

    if (login.empty() || password.empty())
    {
        QMessageBox::warning(this, "Login",
                             "Login and password cannot be empty");
        return;
    }
    if (!clientCore_->loginUser(login, password))
    {
        QMessageBox::warning(this, "Login failed",
                              "Invalid credentials");
        return;
    }
    QMessageBox::information(this, "Login", "Login Successfull");
    loginEdit_->clear();
    passwordEdit_->clear();

    clientCore_->joinChat(login);
    setChatControlsEnabled(true);
    animatedSwitchTo(chatPage_);
}

void ClientGui::onRegistrationClicked()
{
    const std::string newLogin = regLoginEdit_->text().toStdString();
    const std::string newPassword = regPasswordEdit_->text().toStdString();

    if (newLogin.empty() || newPassword.empty())
    {
        QMessageBox::warning(this, "Registration",
                             "Login and password cannot be empty");
        return;
    }

    if (!clientCore_->registerUser(newLogin, newPassword))
    {
        QMessageBox::warning(this, "Registration failed",
                              "User is already registered");
        regLoginEdit_->clear();
        regPasswordEdit_->clear();
        return;
    }
    QMessageBox::information(this, "Registration", "Registration Successfull");
    regLoginEdit_->clear();
    regPasswordEdit_->clear();

    if (!clientCore_->loginUser(newLogin, newPassword))
    {
        QMessageBox::critical(this, "Login failed",
                              "Unknown error");
        return;
    }

    clientCore_->joinChat(newLogin);
    setChatControlsEnabled(true);
    animatedSwitchTo(chatPage_);
}

void ClientGui::onSendClicked()
{
    const auto txt = messageEdit_->text();
    if (txt.isEmpty()) return;

    clientCore_->sendMessage("", "", txt.toStdString());
    messageEdit_->clear();
}

void ClientGui::setChatControlsEnabled(bool en)
{
    history_->setEnabled(en);
    messageEdit_->setEnabled(en);
    sendBtn_->setEnabled(en);
    onlineGroup_->setEnabled(en);
}

void ClientGui::animatedSwitchTo(QWidget *page)
{
    if (stack_->currentWidget() == page) return;

    QWidget *oldPage = stack_->currentWidget();

    auto *fadeOutEff = new QGraphicsOpacityEffect(oldPage);
    oldPage->setGraphicsEffect(fadeOutEff);
    auto *fadeOut = new QPropertyAnimation(fadeOutEff, "opacity", this);
    fadeOut->setDuration(250);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);

    auto *fadeInEff = new QGraphicsOpacityEffect(page);
    page->setGraphicsEffect(fadeInEff);
    fadeInEff->setOpacity(0.0);
    auto *fadeIn = new QPropertyAnimation(fadeInEff, "opacity", this);
    fadeIn->setDuration(250);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);

    connect(fadeOut, &QPropertyAnimation::finished, this, [this, page, fadeIn]() {
        stack_->setCurrentWidget(page);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    });

    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
}

void ClientGui::handleChatEvent(const ChatEvent &ev)
{
    using Type = ChatEvent::Type;
    if (ev.type == Type::CHECK_AVAILABILITY)
    {
        onlineList_->clear();
        for (const auto &user : ev.activeUsers)
            onlineList_->addItem(QString::fromStdString(user));
    }
    else if (ev.type == Type::STANDARD_MESSAGE)
    {
        history_->append(QString::fromStdString(ev.sender) + ": "
                         + QString::fromStdString(ev.message));
    }
}
