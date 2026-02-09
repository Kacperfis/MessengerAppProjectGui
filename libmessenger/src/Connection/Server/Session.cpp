#include "Session.hpp"

namespace connection::session
{

Session::Session(
    boost::asio::io_context& io,
    boost::asio::ip::tcp::socket socket,
    std::shared_ptr<SessionMap> sessions,
    std::shared_ptr<std::mutex> sessionsMutex)
    : socket_(std::move(socket))
    , strand_(boost::asio::make_strand(io))
    , activeSessions_(std::move(sessions))
    , sessionsMutex_(std::move(sessionsMutex))
    , logger_("Session") {}

void Session::start()
{
    receive();
}

void Session::stop()
{
    auto self = shared_from_this();
    boost::asio::post(strand_, [self]() {
        boost::system::error_code ec;
        self->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        self->socket_.close(ec);
    });
}

void Session::receive() {
    auto self = shared_from_this();
    boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(data_), '\n',
    boost::asio::bind_executor(strand_,
    [this, self](boost::system::error_code errorCode, std::size_t length)
    {
        if (!errorCode)
        {
            if (length == 0)
            {
                receive();
                return;
            }

            std::string line(data_.substr(0, length - 1));
            data_.erase(0, length);

            std::string type, sender, recipient, content;
            std::istringstream stringStream(line);
            std::getline(stringStream, type, '|');
            std::getline(stringStream, sender, '|');
            std::getline(stringStream, recipient, '|');
            std::getline(stringStream, content, '|');

            if (type == "ESTABLISH")
            {
                logger_.log(Severity::info, "successfully established session with " + sender);
                std::cout << "successfully established session with " + sender << std::endl;
                
                {
                    std::lock_guard<std::mutex> lock(usernameMutex_);
                    username_ = sender;
                }

                std::vector<std::pair<std::string, std::shared_ptr<Session>>> sessionsToNotify;
                std::set<std::string> activeUsersSet;

                {
                    std::lock_guard<std::mutex> lock(*sessionsMutex_);
                    if (activeSessions_->count(sender) > 0)
                    {
                        stop();
                        return;
                    }
                    (*activeSessions_)[sender] = self;
                    for (const auto& [name, session] : *activeSessions_)
                    {
                        activeUsersSet.insert(name);
                        sessionsToNotify.emplace_back(name, session);
                    }
                }

                for (const auto& [name, session] : sessionsToNotify)
                {
                    auto activeUsers = getActiveUsers(activeUsersSet, name);
                    std::string forwardMessage = "CHECK_AVAILABILITY|" + sender + "|" + name + "|" + activeUsers;
                    logger_.log(Severity::info, "forwarding check availability message to " + name);
                    std::cout << "forwarding check availability message to " + name << std::endl;
                    session->send(forwardMessage);
                }
            } 
            else if (type == "RELINQUISH")
            {
                auto currentUsername = self->getUsername();
                logger_.log(Severity::info, currentUsername + " successfully closed session");
                std::cout << currentUsername + " successfully closed session" << std::endl;

                {
                    std::lock_guard<std::mutex> lock(*sessionsMutex_);
                    activeSessions_->erase(currentUsername);
                }

                stop();
                return;
            }
            else if (type == "MESSAGE")
            {
                std::shared_ptr<Session> recipientSession;
                {
                    std::lock_guard<std::mutex> lock(*sessionsMutex_);
                    auto it = activeSessions_->find(recipient);
                    if (it != activeSessions_->end())
                        recipientSession = it->second;
                }
                
                if (recipientSession) 
                {
                    logger_.log(Severity::info, "got message from " + sender + ", forwarding message to " + recipient);
                    std::cout << "got message from " + sender + ", forwarding message to " + recipient << std::endl;
                    std::string forwardMessage = "MESSAGE|" + sender + "|" + recipient + "|" + content;
                    recipientSession->send(forwardMessage);
                }
                else
                {
                    logger_.log(Severity::info, "got message from " + sender + ", but recipient: " + recipient + " is offline");
                    std::cout << "got message from " + sender + ", but recipient: " + recipient + " is offline" << std::endl;
                }
            }
            else
            {
                logger_.log(Severity::info, "received unknown protocol message, start retrying...");
                std::cout << "received unknown protocol message, start retrying..." << std::endl;
            }
            receive();
        } 
        else
        {
            logger_.log(Severity::info, "connection lost");
            std::cout << "connection lost" << std::endl;
            auto currentUsername = self->getUsername();
            if (!currentUsername.empty())
            {
                std::lock_guard<std::mutex> lock(*sessionsMutex_);
                auto it = activeSessions_->find(currentUsername);
                if (it != activeSessions_->end() && it->second == self)
                    activeSessions_->erase(it);
            }
        }
    }));
}

std::string Session::getActiveUsers(const std::set<std::string>& activeUsers, const std::string& username)
{
    std::string result;
    auto activeUsersCopy = activeUsers;
    auto it = activeUsersCopy.find(username);
    if (it != activeUsersCopy.end())
    {
        activeUsersCopy.erase(it);
    }
    for (const auto& activeUser : activeUsersCopy)
    {
        result += activeUser + ",";
    }

    if (!result.empty() && result.back() == ',')
    {
        result.erase(result.length() - 1);
    }

    return result;
}

std::string Session::getUsername() const
{
    std::lock_guard<std::mutex> lock(usernameMutex_);
    return username_;
}

void Session::send(const std::string& data)
{
    auto currentUsername = getUsername();
    logger_.log(Severity::info, "about to send to " + currentUsername + ": " + data);
    std::cout << "about to send to " + currentUsername + ": " + data << std::endl;

    auto self = shared_from_this();
    auto msg = data + "\n";

    boost::asio::post(self->strand_, [this, self, msg]()
    {
        writeQueue_.push_back(msg);
        if (!isWriting_)
        {
            doWrite();
        }
    });
}

void Session::doWrite()
{
    if (writeQueue_.empty())
    {
        isWriting_ = false;
        return;
    }

    isWriting_ = true;
    auto msg = writeQueue_.front();
    auto self = shared_from_this();
    boost::asio::async_write(socket_, boost::asio::buffer(msg),
        boost::asio::bind_executor(strand_, [this, self](auto ec, auto)
        {
            writeQueue_.pop_front();
            if (!ec)
                doWrite();
        }));
}

} // namespace connection::session
