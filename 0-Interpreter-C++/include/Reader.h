#ifndef READER
#define READER

#include "debug.h"
#include "tools.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Reader
{
private:
	// Paths
	std::string gameDir;
	std::string rcgPath;

	// Data
	std::vector<std::string> rcgData;

public:
	// Constructor and Destructor
	Reader();
	~Reader();

	// private:
	// Managin Paths
	void setPaths(std::string rcgPath);

	// Managin Data
	void loadRcg();
	void loadRcgTeams();
	void loadRcgCycles();
	void loadRcgPlaymodes();
	void savingCSV(std::string saveDir);

public:
	// Calling Reader
	void readGame(std::string rcgPath);
	void readDir(std::string rootDir = "../data");
};

#endif