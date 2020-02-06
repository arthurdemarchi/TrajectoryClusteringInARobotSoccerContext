#ifndef READER
#define READER

#include "debug.h"
#include "tools.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Reader {
private:
  // Paths
  std::string gameDir;
  std::string rclPath;
  std::string rcgPath;

  // Data
  std::vector<std::string> rcgData;

public:
  // Constructor and Destructor
  Reader();
  ~Reader();

private:
  // Managin Paths
  bool setPaths(std::string gameDir);

  // Managin Data
  void loadRcg();
  void savingCSV(std::string saveDir);

public:
  // Calling Reader
  void readGame(std::string gameDir = "../data");
  void readDir(std::string rootDir = "../data");
};

#endif