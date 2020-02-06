#include "Reader.h"

Reader::Reader() {
  // debug("Reader was created for a game in: '" << gameDir << "'.");
}

Reader::~Reader() {
  // debug("Reader from game in: '" << gameDir << "' was destroyed.");
}

bool Reader::setPaths(std::string gameDir) {

  if (!std::filesystem::is_directory(gameDir)) {
    throw std::runtime_error("Invalid Directory");
  }

  rclPath = "";
  rcgPath = "";
  this->gameDir = gameDir;

  for (const auto &entry : std::filesystem::directory_iterator(gameDir)) {
    std::string filePath = entry.path();
    std::string fileExtension =
        filePath.substr(filePath.size() - 4, std::string::npos);
    if (fileExtension == ".rcg") {
      if (rcgPath == "") {
        rcgPath = filePath;
      } else {
        throw std::runtime_error(
            "More than one .rcg file was found in the game directory.");
      }
    }
    if (fileExtension == ".rcl") {
      if (rclPath == "") {
        rclPath = filePath;
      } else {
        throw std::runtime_error(
            "More than one .rcl file was found in the game directory.");
      }
    }
  }

  if (rcgPath == "" || rclPath == "") {
    throw std::runtime_error(
        "No rcl or rcg file was found in the game directory.");
  }

  return true;
}

void Reader::loadRcg() {
  // scope declarations
  std::fstream rcgFile(rcgPath);
  std::string line, playerLine, playerName;
  std::string cycleLines[6000], columns[5];
  std::string patterns[23] = {
      "((b)",   "((l 1)", "((l 2)", "((l 3)",  "((l 4)",  "((l 5)",
      "((l 6)", "((l 7)", "((l 8)", "((l 9)",  "((l 10)", "((l 11)",
      "((r 1)", "((r 2)", "((r 3)", "((r 4)",  "((r 5)",  "((r 6)",
      "((r 7)", "((r 8)", "((r 9)", "((r 10)", "((r 11)"};

  int position;
  int cycle = 0;

  // reading
  while (getline(rcgFile, line)) {
    if (line.find("(show ") != std::string::npos) {
      if (cycle > 1) {
        if (cycleLines[cycle - 1].substr(0, 4) == line.substr(6, 4)) {
          continue;
        }
      }
      cycleLines[cycle] = line.substr(6);
      cycle++;
    }
  }

  // formating
  for (int i = 0; i < 6000; i++) {
    columns[0] = cycleLines[i].substr(0, cycleLines[i].find(" "));
    for (int j = 0; j < 23; j++) {
      if (cycleLines[i].find(patterns[j]) == std::string::npos)
        continue;

      position = cycleLines[i].find(patterns[j]) + patterns[j].size() + 1;
      playerLine = cycleLines[i].substr(position, 64);
      // debug(columns[0] << ": " << playerLine);

      if (j != 0) {
        position = playerLine.find(" ") + 1;
        playerLine = playerLine.substr(position, 64);
        // debug("pre0: " << playerLine);

        position = playerLine.find(" ") + 1;
        playerLine = playerLine.substr(position, 64);
        // debug("pre1: " << playerLine);
      }

      for (int c = 1; c < 5; c++) {
        columns[c] = playerLine.substr(0, playerLine.find(" "));
        // debug(c << "th: " << columns[c]);

        position = playerLine.find(" ") + 1;
        playerLine = playerLine.substr(position, 64);
        // debug("pre1: " << playerLine);
      }

      if (j == 0) {
        playerName = "ball";
        columns[4] = columns[4].substr(0, columns[4].size() - 1);
      } else if (j > 11) {
        playerName = "right " + std::to_string(j - 11);
      } else {
        playerName = "left " + std::to_string(j);
      }

      rcgData.push_back(columns[0] + ", " + playerName + ", " + columns[1] +
                        ", " + columns[2] + ", " + columns[3] + ", " +
                        columns[4]);
    }
  }
}

void Reader::savingCSV(std::string rootDir) {
  // // path
  // gameDir.rfind("/data/");

  //     // scope declarations
  //     std::fstream csvFile;

  // // open file
  // csvFile.open(csvPath, std::ios::out | std::ios::trunc);
  // if (csvFile) {
  // }
  // for (unsigned int i = 0; i < rcgData.size(); i++) {
  //   csvFile << rcgData[i];
  // }
  // csvFile.close();
  // return;
}

void Reader::readGame(std::string gameDir) {
  // get save dir
  std::string saveDir = gameDir;
  int slashPos = saveDir.rfind("/", gameDir.size() - 2);
  saveDir.replace(slashPos, slashPos - gameDir.size(), "/output/");

  // read and save
  try {
    setPaths(gameDir);
    loadRcg();
    savingCSV(saveDir);
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return;
  }
}

void Reader::readDir(std::string rootDir) {
  // get save dir
  std::string saveDir;
  std::string rootSaveDir = rootDir;
  int slashPos = rootSaveDir.rfind("/", rootDir.size() - 2);
  rootSaveDir.replace(slashPos, slashPos - rootDir.size(), "/output");

  // get list of dirs
  std::vector<std::string> gameDirs = listLeaves(rootDir);

  // read all
  for (unsigned int i = 0; i < gameDirs.size(); ++i) {
    try {
      saveDir = gameDirs[i];
      saveDir.replace(0, rootDir.size(), rootSaveDir);

      setPaths(gameDirs[i]);
      loadRcg();
      savingCSV(saveDir);

    } catch (const std::exception &e) {
      std::cerr << "ERROR: " << e.what() << std::endl;
      return;
    }
  }
}