#ifndef READER
#define READER

#include "debug.h"
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
	std::vector<std::string> rcgData;

public:
	// Constructor and Destructor
	Reader();
	~Reader();

private:
	// Managin Paths
	void setAllPaths(std::string rcgPath, std::string rootDir);
	std::vector<std::string> listRcgFiles(std::string rootDir);

	// Managin Data
	void loadRcg();
	void saveCsv();

public:
	// Calling Reader
	void readRcg(std::string rcgPath, std::string rootDir = "");
	void readDir(std::string rootDir = "../data");
};

#endif