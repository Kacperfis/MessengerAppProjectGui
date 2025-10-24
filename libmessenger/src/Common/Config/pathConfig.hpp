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

    const std::string buildMarker = "/build";
    pos = currentPath.rfind(buildMarker);
    if (pos != std::string::npos)
    {
        return currentPath.substr(0, pos + 1);
    }

    // try to find project root by looking for CMakeLists.txt
    auto searchPath = std::filesystem::current_path();
    while (searchPath.has_parent_path() && searchPath != searchPath.root_path())
    {
        if (std::filesystem::exists(searchPath / "CMakeLists.txt") &&
            std::filesystem::exists(searchPath / "libmessenger"))
        {
            return searchPath.string() + "/";
        }
        searchPath = searchPath.parent_path();
    }

    std::cerr << "Marker not found in the current path";
    return "";
}

const std::string pathPrefix = getPathPrefix(std::filesystem::current_path());

} // namespace config::path