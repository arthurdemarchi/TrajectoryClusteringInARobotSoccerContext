#ifndef READER
#define READER

#include "debug.h"
#include <filesystem>
#include <fstream>
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
  Reader(std::string gameDir = Reader::getDefaultReadPath());
  ~Reader();

  // Managin Paths
  static std::string getDefaultReadPath();
  static std::string getDefaultWritePath();
  bool setPaths(std::string gameDir);

  // Managin Data
  void loadRcg();
  void savingCSV(std::string csvPath);
};

#endif