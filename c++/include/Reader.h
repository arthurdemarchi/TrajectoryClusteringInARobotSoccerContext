#pragma once

#include "debug.h"
#include "tools.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define MAX_NUMBER_OF_CYCLES 10000

class Reader
{
private:
	// Paths
	std::string gameDir;
	std::string saveDir;
	std::string rcgName;
	std::string rcgPath;
	std::string csvPath;

	// Data
	std::vector<std::string> csvData;

private:
	void setAllPaths(std::string rcgPath, std::string rootDir);
	void loadRcg();
	void saveCsv();

public:
	// Calling Reader
	void readDir(std::string rootDir = "../data/input");
};