#ifndef TOOLS
#define TOOLS

#include "Reader.h"
#include <filesystem>
#include <string.h>
#include <vector>

std::vector<std::string> listLeaves(std::string rootDir);
void callReaderRecursively(std::string rooDir = NULL);

#endif