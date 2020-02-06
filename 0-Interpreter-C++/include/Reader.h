#ifndef READER
#define READER

#include "debug.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

class Reader {
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