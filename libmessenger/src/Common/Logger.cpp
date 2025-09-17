#include "Logger.hpp"
#include <filesystem>
#include <Common/Config/pathConfig.hpp>

std::string Logger::testName_ = "defaultTestName";

Logger::Logger(const std::string& prefix) : prefix_(prefix)
{
    file_.open(config::path::pathPrefix + "build/ut_logs/" + testName_ + ".log", std::ios::trunc);
    if (!file_.is_open())
    {
        std::cerr << "Unable to open logs file" << std::endl;
    }
}

Logger::~Logger()
{
    if (file_.is_open()) file_.close();
}

void Logger::log(const Severity severity, const std::string& message)
{
    auto currentTime = std::chrono::system_clock::now();
    auto timePoint = std::chrono::system_clock::to_time_t(currentTime);
    std::stringstream ss;
    ss << "[" << severityVec_.at(static_cast<int>(severity)) << "] " << prefix_ << ": " << message << std::endl;
    file_ << ss.str();
}
