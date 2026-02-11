#include "ServerGui.hpp"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QMetaObject>

ServerGui::ServerGui(const std::string& adminDatabasePath, QWidget *parent)
    : QWidget(parent),
      serverCore_(std::make_shared<messengerapp::ServerCore>(8080, adminDatabasePath))
{

    /* ----- basic window setup ----- */
    setWindowTitle("MessengerApp Server");
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
        QLineEdit {
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

    banner_ = new QLabel("MessengerApp Server", this);
    banner_->setAlignment(Qt::AlignCenter);
    banner_->setStyleSheet("font-size: 32px; font-weight: 600; color: #ffffff; "
                           "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0078d7, stop:1 #00d7a7);"
                           "padding: 18px;");

    stack_ = new QStackedWidget(this);
    buildHomePage();
    buildLoginPage();
    buildRegistrationPage();
    buildMainPage();

    auto *root = new QVBoxLayout(this);
    root->setSpacing(0);
    root->setContentsMargins(0,0,0,0);
    root->addWidget(banner_);
    root->addWidget(stack_);
    setLayout(root);

    stack_->setCurrentWidget(homePage_);
}

ServerGui::~ServerGui()
{
    if (serverCore_)
    {
        serverCore_->stopServer();
    }
}

void ServerGui::buildHomePage()
{
    homePage_ = new QWidget(this);
    auto *v = new QVBoxLayout(homePage_);

    homeLoginBtn_    = new QPushButton("Admin Login", homePage_);
    homeRegisterBtn_ = new QPushButton("Admin Register", homePage_);

    v->addStretch(1);
    v->addWidget(homeLoginBtn_);
    v->addWidget(homeRegisterBtn_);
    v->addStretch(2);

    connect(homeLoginBtn_, &QPushButton::clicked,
        this, &ServerGui::onHomeLoginClicked);

    connect(homeRegisterBtn_, &QPushButton::clicked,
        this, &ServerGui::onHomeRegisterClicked);

    stack_->addWidget(homePage_);
}

void ServerGui::buildLoginPage()
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
            this, &ServerGui::onLoginClicked);

    stack_->addWidget(loginPage_);
}

void ServerGui::buildRegistrationPage()
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
        this, &ServerGui::onRegistrationClicked);

    stack_->addWidget(registrationPage_);
}

void ServerGui::buildMainPage()
{
    mainPage_ = new QWidget(this);
    auto *layout = new QVBoxLayout(mainPage_);

    layout->addStretch(1);

    QLabel* title = new QLabel("Admin Panel", this);
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QPushButton* startServerButton = new QPushButton("Start Server", this);
    connect(startServerButton, &QPushButton::clicked,
            this, &ServerGui::onStartServerClicked);

    QPushButton* stopServerButton = new QPushButton("Stop Server", this);
    connect(stopServerButton, &QPushButton::clicked,
            this, &ServerGui::onStopServerClicked);

    layout->addWidget(startServerButton);
    layout->addWidget(stopServerButton);
    layout->addStretch(2);

    stack_->addWidget(mainPage_);
}

void ServerGui::onHomeLoginClicked()
{
    loginEdit_->clear();
    passwordEdit_->clear();
    animatedSwitchTo(loginPage_);
}

void ServerGui::onHomeRegisterClicked()
{
    regLoginEdit_->clear();
    regPasswordEdit_->clear();
    animatedSwitchTo(registrationPage_);
}

void ServerGui::onLoginClicked()
{
    const std::string login    = loginEdit_->text().toStdString();
    const std::string password = passwordEdit_->text().toStdString();

    if (login.empty() || password.empty())
    {
        QMessageBox::warning(this, "Login",
                             "Login and password cannot be empty");
        return;
    }
    bool success = serverCore_->loginAdmin(login, password);

    if (!success)
    {
        QMessageBox::warning(this, "Login failed",
                              "Invalid credentials");
        return;
    }

    loginEdit_->clear();
    passwordEdit_->clear();

    animatedSwitchTo(mainPage_);
}

void ServerGui::onRegistrationClicked()
{
    const std::string newLogin = regLoginEdit_->text().toStdString();
    const std::string newPassword = regPasswordEdit_->text().toStdString();

    if (newLogin.empty() || newPassword.empty())
    {
        QMessageBox::warning(this, "Registration",
                             "Login and password cannot be empty");
        return;
    }

    bool success = serverCore_->registerAdmin(newLogin, newPassword);

    if (!success)
    {
        QMessageBox::warning(this, "Registration failed",
                              "Login already exists or error occurred.");
        regLoginEdit_->clear();
        regPasswordEdit_->clear();
        return;
    }

    regLoginEdit_->clear();
    regPasswordEdit_->clear();

    animatedSwitchTo(loginPage_);
}

void ServerGui::onStartServerClicked()
{
    serverCore_->startServer();
    QMessageBox::information(this, "Server Started", "Server has been started.");
}

void ServerGui::onStopServerClicked()
{
    if (serverCore_)
    {
        serverCore_->stopServer();
        QMessageBox::information(this, "Server Stopped", "Server has been stopped.");
    }
}

void ServerGui::animatedSwitchTo(QWidget *page)
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