#include "Session.hpp"

namespace connection::session
{

Session::Session(boost::asio::io_context& io,
                 boost::asio::ip::tcp::socket socket, 
                 std::map<std::string, std::shared_ptr<Session>>& sessions,
                 std::mutex& sessionsMutex)
    : socket_(std::move(socket))
    , activeSessions_(sessions)
    , sessionsMutex_(sessionsMutex)
    , strand_(boost::asio::make_strand(io))
    , encryptionManager_(std::make_shared<encryption::EncryptionManager>("", "0123456789abcdef0123456789abcdef", "0123456789abcdef"))
    , logger_("Session") {}


void Session::start()
{
    receive();
}

void Session::receive() {
    auto self = shared_from_this();
    boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(data_), '\n',
    boost::asio::bind_executor(strand_,
    [this, self](boost::system::error_code errorCode, std::size_t length)
    {
        if (!errorCode)
        {
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
                username_ = sender;
                
                std::set<std::string> activeUsersSet;
                {
                    std::lock_guard<std::mutex> lock(sessionsMutex_);
                    activeSessions_[username_] = self;
                    for (const auto& activeSession : activeSessions_) 
                        activeUsersSet.insert(activeSession.first);
                }
                
                std::lock_guard<std::mutex> lock(sessionsMutex_);
                for (const auto& activeSession : activeSessions_)
                {
                    auto activeUsers = getActiveUsers(activeUsersSet, activeSession.first);
                    logger_.log(Severity::info, "forwarding check availability message to " + activeSession.first);
                    std::cout << "forwarding check availability message to " + activeSession.first << std::endl;
                    std::string forwardMessage = "CHECK_AVAILABILITY|" + sender + "|" + activeSession.first + "|" + activeUsers;
                    activeSession.second->send(forwardMessage);
                }
            } 
            else if (type == "RELINQUISH")
            {
                logger_.log(Severity::info, username_ + " successfully closed session with " + sender);
                std::cout << username_ + " successfully closed session with " + sender << std::endl;
                std::lock_guard<std::mutex> lock(sessionsMutex_);
                activeSessions_.erase(username_);
            }
            else if (type == "MESSAGE")
            {
                std::shared_ptr<Session> recipientSession;
                {
                    std::lock_guard<std::mutex> lock(sessionsMutex_);
                    auto it = activeSessions_.find(recipient);
                    if (it != activeSessions_.end()) 
                    {
                        recipientSession = it->second;
                    }
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
            std::lock_guard<std::mutex> lock(sessionsMutex_);
            activeSessions_.erase(username_);
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

void Session::send(const std::string& data)
{
    logger_.log(Severity::info, "about to send to " + username_ + ": " + data);
    std::cout << "about to send to " + username_ + ": " + data << std::endl;

    auto self = shared_from_this();
    std::string message = data + "\n";
    
    boost::asio::post(strand_,
    [self, msg = std::move(message)]()
    {
        boost::asio::async_write(self->socket_, boost::asio::buffer(msg),
        boost::asio::bind_executor(self->strand_,
        [self, msg](boost::system::error_code errorCode, std::size_t length)
        {
            if (errorCode)
            {
                self->logger_.log(Severity::error, "error sending to " + self->username_ + ": " + errorCode.message());
            } 
            else
            {
                self->logger_.log(Severity::info, "sent " + std::to_string(length) + " bytes to " + self->username_ + ": " + msg);
            }
        }));
    });
}

} // namespace connection::session
