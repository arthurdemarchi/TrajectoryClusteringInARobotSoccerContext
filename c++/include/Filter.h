#include "debug.h"
#include "tools.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <math.h>

//defining player maximum length
#define MAX_PLAY_LENGTH 150

//defining manual book constants
#define PLAYER_BODY 0.085
#define PLAYER_SIZE 0.3
#define PLAYER_AREA (PLAYER_BODY + PLAYER_SIZE)
#define KICK_MARGIN 0.7
#define ERROR_MARGIN 1.05
#define KICKABLE_AREA (PLAYER_AREA + KICK_MARGIN) * ERROR_MARGIN
#define PLAYER_MOVEMENT 1.2 * ERROR_MARGIN
#define DASH_DISTANCE PLAYER_MOVEMENT * 2
#define REACTION_TIME 1.5
#define REACTION 2 * (KICKABLE_AREA) / REACTION_TIME
#define INTERACTION_TRESHOLD 0.4

//filter class
//used to filter csv interpreted
//results.
class Filter
{
private:
	// Paths
	std::string gameDir;
	std::string saveDir;
	std::string inputName;
	std::string inputPath;
	std::string outputPath;

	// Data
	std::vector<std::vector<float>> data;
	std::vector<std::vector<int>> plays;
	std::vector<std::vector<float>> playsData;
	std::vector<std::vector<float>> pathsData;

	//aux data
	std::string inputLine;
	std::vector<float> dataLine;
	std::vector<float> path;
	std::vector<int> play;

	// methods

	// paths
	void setAllPaths(std::string inputPath, std::string rootDir);

	// data
	void loadData();
	void createPaths();
	void saveCsv();

	// filtering
	void filter();
	void evalHold();

	// aux methods
	bool isAnEnd(int i);
	int lookForBegin(int i);
	int playmodeToFloat(std::string playmode);

public:
	//using class
	void filterDir(std::string rootDir = "../data/csv");
	void saveRaw();
};