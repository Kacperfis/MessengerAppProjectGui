#pragma once
#include <string>
#include <iostream>
#include <sstream>

class NullBuffer : public std::streambuf {};
class NullStream : public std::ostream
{
public:
    NullStream();
private:
    NullBuffer someBuffer_;
};

void clearFile(const std::string& path);
void clearAllFilesFromFolder(const std::string& folderPath);
void setDefaultValueForIndexFile(const std::string& filePath, const std::string& value);