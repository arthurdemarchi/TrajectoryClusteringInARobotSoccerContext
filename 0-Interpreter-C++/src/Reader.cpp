#include "Reader.h"

Reader::Reader()
{
	// debug("Reader was created for a game in: '" << gameDir << "'.");
}

Reader::~Reader()
{
	// debug("Reader from game in: '" << gameDir << "' was destroyed.");
}

void Reader::setAllPaths(std::string rcgPath, std::string rootDir)
{
	if (!(rcgPath.rfind(".rcg") == rcgPath.size() - 4))
		throw std::runtime_error("Not a .rcg file.");

	if (!std::filesystem::exists(rcgPath))
		throw std::runtime_error("File doesnt exist.");

	// Paths
	this->rcgPath = rcgPath;
	this->gameDir = rcgPath.substr(0, rcgPath.rfind("/"));
	this->rcgName = rcgPath.substr(gameDir.size() + 1);

	if (rootDir == "")
	{
		rootDir = gameDir;
	}

	int position = rootDir.rfind("/", saveDir.size() - 2);
	this->saveDir = rootDir.substr(0, position);
	this->saveDir = saveDir + "/output" + gameDir.substr(rootDir.size());

	this->csvPath = saveDir + "/" + rcgName;
	this->csvPath.replace(csvPath.size() - 4, 4, ".csv");
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

	// preaparing table to insert data
	rcgData.clear();
	rcgData.push_back("cycle, object, position x, position y, speed x, speed y, playmode \n");

	// reading
	while (getline(rcgFile, line))
	{
		//reading player and ball cylcles
		if (line.find("(show ") != std::string::npos)
		{
			cycle = std::stoi(line.substr(6, 4));
			cycleLines[cycle] = line.substr(6);
		}
		//reading result(team and score) line
		if (line.find("(result ") != std::string::npos)
			resultLine = line;
		//reading playmode lines
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
	for (int cycle = 0; cycle < MAX_NUMBER_OF_CYCLES; cycle++)
	{
		columns[0] = cycleLines[cycle].substr(0, cycleLines[cycle].find(" "));
		columns[5] = playmodeLines[cycle];

		for (int j = 0; j < 23; j++)
		{
			if (cycleLines[cycle].find(patterns[j]) == std::string::npos)
				continue;

			position = cycleLines[cycle].find(patterns[j]) + patterns[j].size() + 1;
			playerLine = cycleLines[cycle].substr(position, 64);

			if (j != 0)
			{
				position = playerLine.find(" ") + 1;
				playerLine = playerLine.substr(position, 64);

				position = playerLine.find(" ") + 1;
				playerLine = playerLine.substr(position, 64);
			}

			for (int c = 1; c < 5; c++)
			{
				columns[c] = playerLine.substr(0, playerLine.find(" "));

				position = playerLine.find(" ") + 1;
				playerLine = playerLine.substr(position, 64);
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

			//writing to class data
			rcgData.push_back(columns[0] + ", " + playerName + ", " + columns[1] +
												", " + columns[2] + ", " + columns[3] + ", " +
												columns[4] + ", " + columns[5] + "\n");
		}
	}
}

void Reader::saveCsv()
{

	// check if dir exists and creates it if it doesnt.
	if (!std::filesystem::exists(saveDir))
		std::filesystem::create_directories(saveDir);

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

void Reader::readRcg(std::string rcgPath, std::string rootDir)
{
	try
	{
		setAllPaths(rcgPath, rootDir);
		loadRcg();
		saveCsv();
	}
	catch (const std::exception &e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return;
	}
}

void Reader::readDir(std::string rootDir)
{
	std::vector<std::string> rcgPaths = listRcgFiles(rootDir);
	for (unsigned int i = 0; i < rcgPaths.size(); ++i)
	{
		std::cout << "reading file: " << rcgPaths[i] << std::endl;
		readRcg(rcgPaths[i], rootDir);
	}
}

std::vector<std::string> Reader::listRcgFiles(std::string rootDir)
{
	std::vector<std::string> listOfRcgs;
	std::string entryPath;
	for (const auto &entry : std::filesystem::recursive_directory_iterator(rootDir))
	{

		if (std::filesystem::is_directory(entry.path()))
			continue;

		entryPath = entry.path();
		if (!(entryPath.rfind(".rcg") == entryPath.size() - 4))
			continue;

		listOfRcgs.push_back(entry.path());
	}
	return listOfRcgs;
};