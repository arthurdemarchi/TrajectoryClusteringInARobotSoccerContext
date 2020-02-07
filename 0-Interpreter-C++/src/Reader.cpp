#include "Reader.h"
#define MAX_NUMBER_OF_CYCLES 10000
Reader::Reader()
{
	// debug("Reader was created for a game in: '" << gameDir << "'.");
}

Reader::~Reader()
{
	// debug("Reader from game in: '" << gameDir << "' was destroyed.");
}

void Reader::setPaths(std::string rcgPath)
{
	if (!(rcgPath.rfind(".rcg") == rcgPath.size() - 4))
		throw std::runtime_error("Not a .rcg file.");

	if (!std::filesystem::exists(rcgPath))
		throw std::runtime_error("File doesnt exist.");

	this->rcgPath = rcgPath;
	gameDir = rcgPath.substr(0, rcgPath.rfind("/"));
}

void Reader::loadRcg()
{
	// scope declarations
	std::fstream rcgFile(rcgPath);
	std::string line, playerLine, playerName, resultLine, leftTeam, rightTeam;
	std::string cycleLines[MAX_NUMBER_OF_CYCLES], playmodeLines[MAX_NUMBER_OF_CYCLES], columns[6];
	std::string patterns[23] = {
			"((b)", "((l 1)", "((l 2)", "((l 3)", "((l 4)", "((l 5)",
			"((l 6)", "((l 7)", "((l 8)", "((l 9)", "((l 10)", "((l 11)",
			"((r 1)", "((r 2)", "((r 3)", "((r 4)", "((r 5)", "((r 6)",
			"((r 7)", "((r 8)", "((r 9)", "((r 10)", "((r 11)"};
	int position;
	int cycle = 0;
	rcgData.clear();
	rcgData.push_back("cycle, object, position x, position y, speed x, speed y, playmode \n");

	// reading
	while (getline(rcgFile, line))
	{
		if (line.find("(show ") != std::string::npos)
		{
			cycle = std::stoi(line.substr(6, 4));
			// debug(cycle);
			cycleLines[cycle] = line.substr(6);
		}
		if (line.find("(result ") != std::string::npos)
		{
			resultLine = line;
		}
		if (line.find("(playmode ") != std::string::npos)
		{
			cycle = std::stoi(line.substr(line.find(" ") + 1, line.rfind(" ") - line.find(" ")));
			playmodeLines[cycle] = line.substr(line.rfind(" ") + 1, std::string::npos);
			playmodeLines[cycle].pop_back();
		}
		else
		{
			if (playmodeLines[cycle].empty())
			{
				if (playmodeLines[cycle - 1] == "play_on")
					playmodeLines[cycle] = "play_on";
				else
				{
					playmodeLines[cycle] = "stopped";
				}
			}
		}
	}

	// discovering teams
	resultLine = resultLine.substr(resultLine.find("(result ") + 8);
	resultLine = resultLine.substr(resultLine.find(" ") + 1);
	leftTeam = resultLine.substr(0, resultLine.find("-vs-"));
	leftTeam = leftTeam.substr(0, leftTeam.find_last_of('_'));
	rightTeam = resultLine.substr(resultLine.find("-vs-") + 4);
	rightTeam = rightTeam.substr(0, rightTeam.find_last_of('_'));

	// formating
	for (int i = 0; i < MAX_NUMBER_OF_CYCLES; i++)
	{
		columns[0] = cycleLines[i].substr(0, cycleLines[i].find(" "));
		columns[5] = playmodeLines[i];

		for (int j = 0; j < 23; j++)
		{
			if (cycleLines[i].find(patterns[j]) == std::string::npos)
				continue;

			position = cycleLines[i].find(patterns[j]) + patterns[j].size() + 1;
			playerLine = cycleLines[i].substr(position, 64);
			// debug(columns[0] << ": " << playerLine);

			if (j != 0)
			{
				position = playerLine.find(" ") + 1;
				playerLine = playerLine.substr(position, 64);
				// debug("pre0: " << playerLine);

				position = playerLine.find(" ") + 1;
				playerLine = playerLine.substr(position, 64);
				// debug("pre1: " << playerLine);
			}

			for (int c = 1; c < 5; c++)
			{
				columns[c] = playerLine.substr(0, playerLine.find(" "));
				// debug(c << "th: " << columns[c]);

				position = playerLine.find(" ") + 1;
				playerLine = playerLine.substr(position, 64);
				// debug("pre1: " << playerLine);
			}

			if (j == 0)
			{
				playerName = "ball";
				columns[4] = columns[4].substr(0, columns[4].size() - 1);
			}
			else if (j > 11)
			{
				playerName = rightTeam + " " + std::to_string(j - 11);
			}
			else
			{
				playerName = leftTeam + " " + std::to_string(j);
			}

			rcgData.push_back(columns[0] + ", " + playerName + ", " + columns[1] +
												", " + columns[2] + ", " + columns[3] + ", " +
												columns[4] + ", " + columns[5] + "\n");
		}
	}
}

void Reader::savingCSV(std::string saveDir)
{

	// check if dir exists and creates it if it doesnt.
	if (!std::filesystem::exists(saveDir))
		std::filesystem::create_directories(saveDir);

	//csvPath
	std::string gameName = rcgPath;
	gameName.replace(0, gameDir.size(), "");
	std::string csvPath = saveDir + gameName;
	csvPath.replace(csvPath.size() - 4, 4, ".csv");

	// scope declarations
	std::fstream csvFile;
	csvFile.open(csvPath, std::ios::out | std::ios::trunc);
	for (unsigned int i = 0; i < rcgData.size(); i++)
	{
		csvFile << rcgData[i];
	}
	csvFile.close();
	return;
}

void Reader::readGame(std::string rcgPath)
{
	// get save dir
	std::string saveDir = gameDir;
	int slashPos = saveDir.rfind("/", gameDir.size() - 2);
	saveDir.replace(slashPos, slashPos - gameDir.size(), "/output/");

	// read and save
	try
	{
		setPaths(rcgPath);
		loadRcg();
		savingCSV(saveDir);
	}
	catch (const std::exception &e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return;
	}
}

void Reader::readDir(std::string rootDir)
{
	// get save dir
	std::string saveDir;
	std::string rootSaveDir = rootDir;
	int slashPos = rootSaveDir.rfind("/", rootDir.size() - 2);
	rootSaveDir.replace(slashPos, slashPos - rootDir.size(), "/output");

	// get list of dirs
	std::vector<std::string> rcgPaths = listLeaves(rootDir);

	// read all
	for (unsigned int i = 0; i < rcgPaths.size(); ++i)
	{
		std::cout << "reading file: " << rcgPaths[i] << std::endl;
		try
		{
			setPaths(rcgPaths[i]);
			saveDir = gameDir;
			saveDir.replace(0, rootDir.size(), rootSaveDir);
			loadRcg();
			savingCSV(saveDir);
		}
		catch (const std::exception &e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl;
			return;
		}
	}
}