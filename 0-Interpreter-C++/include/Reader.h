#ifndef READER
#define READER

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include "debug.h"

class Reader
{
private:
    std::string gameDir;
    std::string rclPath;
    std::string rcgPath;

public:
    static std::string getDefaultPath();
    Reader(std::string gameDir = Reader::getDefaultPath());
    bool setPaths(std::string gameDir);
    void openFiles();
    ~Reader();
};

#endif