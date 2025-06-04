#pragma once

#include <string>
#include <filesystem>

namespace config::path
{

inline std::string getPathPrefix(const std::filesystem::path& path)
{
    auto currentPath = path.string();
    const std::string marker = "new/";
    size_t pos = currentPath.rfind(marker);
    if (pos != std::string::npos)
    {
        return currentPath.substr(0, pos + marker.length());
    }
    else
    {
        std::cerr << "Marker not found in the current path";
        return "";
    }
}

const std::string pathPrefix = "/" + getPathPrefix(std::filesystem::current_path().relative_path());

} // namespace config::path