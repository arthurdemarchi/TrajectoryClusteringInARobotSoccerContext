#include "Filter.h"

void Filter::setAllPaths(std::string inputPath, std::string rootDir)
{
	if (!(inputPath.rfind(".csv") == inputPath.size() - 4))
		throw std::runtime_error("Not a .csv file.");

	if (!std::filesystem::exists(inputPath))
		throw std::runtime_error("File doesnt exist.");

	// Paths
	this->inputPath = inputPath;
	this->gameDir = inputPath.substr(0, inputPath.rfind("/"));
	this->inputName = inputPath.substr(gameDir.size() + 1);

	this->saveDir = rootDir.substr(0, rootDir.rfind("/", saveDir.size() - 2));
	this->saveDir = saveDir + "/filtered" + gameDir.substr(rootDir.size());

	this->outputPath = saveDir + "/" + inputName;
}

void Filter::loadData()
{
	// open file
	std::fstream inputFile(inputPath);
	int position;
	float cycle = 0, team, player, pos_x, pos_y, speed_x, speed_y, playmode;
	std::string teamName;

	// DATA STRUCTURE
	// CYCCLE  TEAM  PLAYER  POS_X  POS_Y  SPEED_X  SPEED_Y  IHOLD  FHOLD  HOLDING  PLAYMODE
	//clear data
	data.clear();
	//throws header away
	getline(inputFile, inputLine);
	while (getline(inputFile, inputLine))
	{
		//cycle
		position = inputLine.find(" ");
		cycle = std::stof(inputLine.substr(0, position));
		inputLine = inputLine.substr(position + 1);

		//team
		position = inputLine.find(" ");
		if (inputLine.substr(0, position) == "ball,")
		{
			team = -1;
		}
		else if (!(teamName == inputLine.substr(0, position)))
		{
			team++;
			teamName = inputLine.substr(0, position);
		}
		inputLine = inputLine.substr(position + 1);

		//player
		if (team == -1)
		{
			player = 0;
		}
		else
		{
			position = inputLine.find(" ");
			player = std::stof(inputLine.substr(0, position));
			inputLine = inputLine.substr(position + 1);
		}

		//pos_x
		position = inputLine.find(" ");
		pos_x = std::stof(inputLine.substr(0, position));
		inputLine = inputLine.substr(position + 1);

		//pos_y
		position = inputLine.find(" ");
		pos_y = std::stof(inputLine.substr(0, position));
		inputLine = inputLine.substr(position + 1);

		//speed_x
		position = inputLine.find(" ");
		speed_x = std::stof(inputLine.substr(0, position));
		inputLine = inputLine.substr(position + 1);

		//speed_y
		position = inputLine.find(" ");
		speed_y = std::stof(inputLine.substr(0, position));
		inputLine = inputLine.substr(position + 1);

		playmode = playmodeToFloat(inputLine);

		dataLine.clear();
		dataLine.push_back(cycle);
		dataLine.push_back(team);
		dataLine.push_back(player);
		dataLine.push_back(pos_x);
		dataLine.push_back(pos_y);
		dataLine.push_back(speed_x);
		dataLine.push_back(speed_y);
		dataLine.push_back(0); //ihold
		dataLine.push_back(0); //fhold
		dataLine.push_back(0); //holding
		dataLine.push_back(playmode);
		data.push_back(dataLine);
	}
}

int Filter::playmodeToFloat(std::string playmode)
{
	if (playmode == "play_on")
	{
		return 0;
	}
	if (playmode == "stopped")
	{
		return 1;
	}
	if (playmode == "free_kick_l")
	{
		return 2;
	}
	if (playmode == "free_kick_r")
	{
		return 3;
	}
	if (playmode == "kick_in_l")
	{
		return 4;
	}
	if (playmode == "kick_in_r")
	{
		return 5;
	}
	if (playmode == "offside_l")
	{
		return 6;
	}
	if (playmode == "offside_r")
	{
		return 7;
	}
	if (playmode == "foul_charge_l")
	{
		return 8;
	}
	if (playmode == "foul_charge_r")
	{
		return 9;
	}
	if (playmode == "kick_off_l")
	{
		return 10;
	}
	if (playmode == "kick_off_r")
	{
		return 11;
	}
	if (playmode == "goal_kick_r")
	{
		return 12;
	}
	if (playmode == "goal_kick_l")
	{
		return 13;
	}
	if (playmode == "goal_l")
	{
		return 14;
	}
	if (playmode == "goal_r")
	{
		return 15;
	}
	if (playmode == "corner_kick_l")
	{
		return 16;
	}
	if (playmode == "corner_kick_r")
	{
		return 17;
	}
	if (playmode == "back_pass_l")
	{
		return 18;
	}
	if (playmode == "back_pass_r")
	{
		return 19;
	}
	if (playmode == "indirect_free_kick_l")
	{
		return 20;
	}
	if (playmode == "indirect_free_kick_r")
	{
		return 21;
	}
	if (playmode == "back_pass_l")
	{
		return 22;
	}
	if (playmode == "back_pass_r")
	{
		return 23;
	}
	if (playmode == "penalty_setup_l")
	{
		return 24;
	}
	if (playmode == "penalty_setup_r")
	{
		return 25;
	}
	if (playmode == "penalty_ready_l")
	{
		return 26;
	}
	if (playmode == "penalty_ready_r")
	{
		return 27;
	}
	if (playmode == "penalty_taken_l")
	{
		return 28;
	}
	if (playmode == "penalty_taken_r")
	{
		return 29;
	}
	if (playmode == "penalty_score_l")
	{
		return 30;
	}
	if (playmode == "penalty_score_r")
	{
		return 31;
	}
	if (playmode == "penalty_miss_l")
	{
		return 32;
	}
	if (playmode == "penalty_miss_r")
	{
		return 33;
	}
	if (playmode == "free_kick_fault_l")
	{
		return 33;
	}
	if (playmode == "free_kick_fault_r")
	{
		return 34;
	}
	if (playmode == "time_over")
	{
		return 35;
	}

	std::cout << playmode << std::endl;
	throw std::runtime_error("invalid playmode.");
	return -1;
}

void Filter::evalHold()
{
	bool holdFlag = false;
	int ballLine = 0;
	float distFromBall = 0, relSpeed = 0, ballAccelaration = 0, ballSpeed = 0;
	float pos_x = 0, pos_y = 0, speed_x = 0, speed_y = 0;
	float b_pos_x = 0, b_pos_y = 0, b_speed_x = 0, b_speed_y = 0;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (i % 23 == 0)
		{
			ballLine = i;
			b_pos_x = data[ballLine][3];
			b_pos_y = data[ballLine][4];
			b_speed_x = data[ballLine][5];
			b_speed_y = data[ballLine][6];
			data[ballLine][7] = 0;
			data[ballLine][8] = 0;
			continue;
		}

		pos_x = data[i][3];
		pos_y = data[i][4];
		speed_x = data[i][5];
		speed_y = data[i][6];

		distFromBall = pow(pow((pos_x - b_pos_x), 2) + pow((pos_y - b_pos_y), 2), 0.5);
		relSpeed = pow(pow((speed_x - b_speed_x), 2) + pow((speed_y - b_speed_y), 2), 0.5);
		ballAccelaration = ballSpeed - pow(pow(b_speed_x, 2) + pow(b_speed_y, 2), 0.5);
		ballSpeed = pow(pow(b_speed_x, 2) + pow(b_speed_y, 2), 0.5);

		//iHold
		if (distFromBall <= PLAYER_AREA)
		{
			data[i][7] = true;
		}
		else if ((distFromBall <= KICKABLE_AREA) and (relSpeed <= REACTION or ballAccelaration > INTERACTION_TRESHOLD))
		{
			data[i][7] = true;
		}
		else
		{
			data[i][7] = false;
		}

		//fHold
		if (distFromBall > DASH_DISTANCE)
		{
			data[i][8] = true;
		}
		else if (distFromBall > KICKABLE_AREA and ballSpeed > PLAYER_MOVEMENT)
		{
			data[i][8] = true;
		}
		else
		{
			data[i][8] = false;
		}
	}

	//holding
	holdFlag = false;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (i > 23)
		{
			data[i][9] = data[i - 23][9];
		}
		if (data[i][8])
		{
			if (data[i][9])
				holdFlag = false;
			data[i][9] = false;
		}
		if (data[i][7] and !holdFlag)
		{
			data[i][9] = true;
			holdFlag = true;
		}
	}
}

void Filter::filter()
{
	//get plays
	plays.clear();
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (isAnEnd(i) and !isAnEnd(i - 1))
		{
			play.clear();
			play.push_back(lookForBegin(i));
			play.push_back(data[i][0]);
			plays.push_back(play);
		}
	}

	//filter using plays
	unsigned int playIndex = 0;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i][0] >= plays[playIndex][0] and data[i][0] < plays[playIndex][1])
		{
			playsData.push_back(data[i]);
		}
		if (data[i][0] >= plays[playIndex][1])
		{
			playIndex++;
			if (playIndex == plays.size())
				break;
			i--;
		}
	}
}

bool Filter::isAnEnd(int i)
{
	if (i < 2)
	{
		return false;
	}

	//check goalie kick off
	if ((data[i][10] == 12 or data[i][10] == 13) and (data[i - 1][10] == 12 or data[i - 1][10] == 13))
		return true;

	// // 	checkGoal();
	if ((data[i][10] == 14 or data[i][10] == 15) and (data[i - 1][10] == 14 or data[i - 1][10] == 15))
		return true;

	// check corner kick off
	if ((data[i][10] == 16 or data[i][10] == 17) and (data[i - 1][10] == 16 or data[i - 1][10] == 17))
		return true;

	//check goalie catch
	if ((data[i][10] == 2 or data[i][10] == 3) and data[i][9] == 1 and data[i][2] == 1)
		return true;

	return false;
}

int Filter::lookForBegin(int i)
{
	int teamOnBall = 0;

	if (i < 23 * 3)
	{
		return 0;
	}

	for (int j = i - 23 * 3; j < i - 23 * 2; j++)
	{
		if (data[j][9])
			teamOnBall = data[j][1];
	}

	for (int j = i; j > i - (MAX_PLAY_LENGTH * 23); j--)
	{
		if (j == 0)
		{
			return 0;
		}
		//stop ball
		//any play mode that not play_on
		if (data[j][10] and !data[j + 1][10])
		{
			return data[j + 1][0];
		}

		//change on team on ball
		if (data[j][9] and !(teamOnBall == data[j][1]))
			return data[j + 1][0];
	}

	//max play length
	return data[i - MAX_PLAY_LENGTH * 23][0];
}

void Filter::createPaths()
{
	pathsData.clear();
	path.clear();

	std::vector<int> playFirstLine;
	playFirstLine.push_back(0);
	for (unsigned int i = 1; i < playsData.size(); i++)
	{
		if (playsData[i][0] - playsData[i - 1][0] > 1)
		{
			playFirstLine.push_back(i);
		}
	}

	for (unsigned int plf = 0; plf < playFirstLine.size(); plf++)
	{
		for (int player = 0; player < 23; player++)
		{
			path.push_back(playsData[player + playFirstLine[plf]][0]);
			path.push_back(playsData[player + playFirstLine[plf]][1]);
			path.push_back(playsData[player + playFirstLine[plf]][2]);
			unsigned int delta = player + playFirstLine[plf];
			for (unsigned int i = delta; i < playsData.size(); i = i + 23)
			{
				if ((i > delta + 23 and (playsData[i][0] - playsData[i - 23][0] != 1)))
				{
					pathsData.push_back(path);
					path.clear();
					break;
				}

				path.push_back(playsData[i][3]);
				path.push_back(playsData[i][4]);
				path.push_back(playsData[i][5]);
				path.push_back(playsData[i][6]);

				if ((i > playsData.size() - 24))
				{
					pathsData.push_back(path);
					path.clear();
					break;
				}
			}
		}
	}
}

void Filter::saveCsv()
{
	// check if dir exists and creates it if it doesnt.
	if (!std::filesystem::exists(saveDir))
		std::filesystem::create_directories(saveDir);

	// scope declarations
	std::fstream csvFile;
	csvFile.open(outputPath, std::ios::out | std::ios::trunc);

	//headers
	csvFile << "cycle, team, player, ";
	for (int i = 0; i < MAX_PLAY_LENGTH; i++)
	{
		csvFile << "pos_x_" << i << ", "
						<< "pos_y_" << i << ", speed_x_" << i << ", speed_y_" << i;
		if (i != MAX_PLAY_LENGTH - 1)
		{
			csvFile << ", ";
		}
	}
	csvFile << std::endl;

	//print data
	for (unsigned int i = 0; i < pathsData.size(); i++)
	{
		for (unsigned int j = 0; j < pathsData[i].size(); j++)
		{
			csvFile << pathsData[i][j];
			if (j != pathsData[i].size() - 1)
				csvFile << ", ";
		}
		csvFile << std::endl;
	}
	csvFile.close();
	return;
}

void Filter::saveRaw()
{
	// check if dir exists and creates it if it doesnt.
	saveDir.replace(saveDir.rfind("/filtered"), 10, "/raw/");
	if (!std::filesystem::exists(saveDir))
		std::filesystem::create_directories(saveDir);

	// scope declarations
	std::fstream csvFile;
	outputPath = outputPath.substr(0, outputPath.size() - 4);
	outputPath.replace(outputPath.rfind("/filtered/"), 10, "/raw/");
	csvFile.open(outputPath, std::ios::out | std::ios::trunc);

	//print data
	for (unsigned int i = 0; i < pathsData.size(); i++)
	{
		for (unsigned int j = 3; j < pathsData[i].size(); j++)
		{
			csvFile << pathsData[i][j];
			if (j != pathsData[i].size() - 1)
				csvFile << ", ";
		}
		csvFile << std::endl;
	}
	csvFile.close();
	return;
}

void Filter::filterDir(std::string rootDir)
{
	std::vector<std::string> inputPaths = listFiles(rootDir, ".csv");
	for (unsigned int i = 0; i < inputPaths.size(); ++i)
	{
		std::cout << i << ". reading file: " << inputPaths[i] << std::endl;
		try
		{
			std::cout << "\t" << i << ".1. setting paths and attributes." << std::endl;
			setAllPaths(inputPaths[i], rootDir);
			std::cout << "\t" << i << ".2. loading file: '" << inputName << "'." << std::endl;
			loadData();
			std::cout << "\t" << i << ".3. evaluating ball possession." << std::endl;
			evalHold();
			std::cout << "\t" << i << ".4. filtering data." << std::endl;
			filter();
			std::cout << "\t" << i << ".5. creating paths." << std::endl;
			createPaths();
			std::cout << "\t" << i << ".6. writing to I/O." << std::endl;
			saveCsv();
			std::cout << "\t" << i << "7. writin raw to I/O" << std::endl;
			saveRaw();
		}
		catch (const std::exception &e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl;
			return;
		}
	}
}
