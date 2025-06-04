#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>

enum class Severity
{
    info,
    debug,
    warning,
    error
};

class Logger
{
public:
    Logger(const std::string& prefix);
    ~Logger();
    void log(const Severity severity, const std::string& message);
    static std::string testName_;
private:
    std::ofstream file_;
    std::string prefix_;
    std::vector<std::string> severityVec_ = {"info", "debug", "warning", "error"};
};
