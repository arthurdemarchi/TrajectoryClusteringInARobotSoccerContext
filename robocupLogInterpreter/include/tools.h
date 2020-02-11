#pragma once
#include <vector>
#include <filesystem>
#include <string>

// function to list all files of one specified extension into
// a root directory
std::vector<std::string> listFiles(std::string rootDir, std::string extension);