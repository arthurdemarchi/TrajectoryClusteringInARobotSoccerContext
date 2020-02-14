#include "Filter.h"

// SET SYSTEM PATHS
void Filter::setAllPaths(const std::string &inputPath, const std::string &rootDir)
{
	// tests if files arguments are valid
	if (!(inputPath.rfind(".csv") == inputPath.size() - 4))
		throw std::runtime_error("Not a .csv file.");

	if (!std::filesystem::exists(inputPath))
		throw std::runtime_error("File doesnt exist.");

	// sets all system paths attributes
	//input
	this->inputPath = inputPath;
	this->inputDir = inputPath.substr(0, inputPath.rfind("/"));
	this->inputName = inputPath.substr(inputDir.size() + 1);
	//filtered
	this->filteredDir = rootDir.substr(0, rootDir.rfind("/", filteredDir.size() - 2));
	this->filteredDir = filteredDir + "/filtered" + inputDir.substr(rootDir.size());
	this->filteredPath = filteredDir + "/" + inputName;
	//raw
	this->rawDir = filteredDir;
	this->rawDir = rawDir.replace(rawDir.rfind("/filtered"), 9, "/raw");
	this->rawPath = rawDir + "/" + inputName.substr(0, inputName.size() - 4);
}

//SET DATA
void Filter::loadData()
{
	// open file
	std::fstream inputFile(inputPath);

	//scope declarations
	int position;
	float cycle = 0, team, player, pos_x, pos_y, speed_x, speed_y, playmode;
	std::string teamName;

	//clear data before writing new loaded data
	data.clear();

	//discarts headers from csv file
	getline(inputFile, inputLine);

	//while for each line
	while (getline(inputFile, inputLine))
	{
		//get cycle from line
		position = inputLine.find(" ");
		cycle = std::stof(inputLine.substr(0, position));
		inputLine = inputLine.substr(position + 1);

		//get team from line converts to float
		//-1 = ball, 0 = left, 1 = right
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

		//get player from line
		//ball is considered player 0
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

		//following could be a for loop, but that is less
		//int terms of wich data is being loaded and
		//has no performance advantadge.

		//get pos_x
		position = inputLine.find(" ");
		pos_x = std::stof(inputLine.substr(0, position));
		inputLine = inputLine.substr(position + 1);

		//get pos_y
		position = inputLine.find(" ");
		pos_y = std::stof(inputLine.substr(0, position));
		inputLine = inputLine.substr(position + 1);

		//get speed_x
		position = inputLine.find(" ");
		speed_x = std::stof(inputLine.substr(0, position));
		inputLine = inputLine.substr(position + 1);

		//get speed_y
		position = inputLine.find(" ");
		speed_y = std::stof(inputLine.substr(0, position));
		inputLine = inputLine.substr(position + 1);

		//get playmode and converts to a float via function
		playmode = playmodeToFloat(inputLine);

		//clear line before loading
		//loads line
		//laods data
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

int Filter::playmodeToFloat(const std::string &playmode)
{
	// there are 35 different playmodes
	// those are translated here to float.
	// 0 (falta) is play_on, game rolling
	// any other thing (true) is a deadball
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

//SET PLAYS
void Filter::setPlays()
{
	//clear plays before loading
	plays.clear();
	//for each data line, check if a
	//line is tthe finalization of play
	//and the line before is not
	//than finds the beginning of said play
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
}

void Filter::evalHold()
{
	//scope declaration to facilitate readability
	bool holdFlag = false;
	int ballLine = 0;
	float distFromBall = 0, relSpeed = 0, ballAccelaration = 0, ballSpeed = 0;
	float pos_x = 0, pos_y = 0, speed_x = 0, speed_y = 0;
	float b_pos_x = 0, b_pos_y = 0, b_speed_x = 0, b_speed_y = 0;

	//stores ball data for a cycle
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

		//uses balls and players data to define important values sucha s relative speed and others
		//to after that use said values to infer ball posession.
		distFromBall = pow(pow((pos_x - b_pos_x), 2) + pow((pos_y - b_pos_y), 2), 0.5);
		relSpeed = pow(pow((speed_x - b_speed_x), 2) + pow((speed_y - b_speed_y), 2), 0.5);
		ballAccelaration = ballSpeed - pow(pow(b_speed_x, 2) + pow(b_speed_y, 2), 0.5);
		ballSpeed = pow(pow(b_speed_x, 2) + pow(b_speed_y, 2), 0.5);

		//iHold - defines begin of possesson (data seventh column)
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

		//fHold - defines end of poessions (data eigth column)
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

	//holding - using beging and end of possession, defines possession
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

bool Filter::isAnEnd(int i)
{
	//if its to early in a game cant be an end of play
	if (i < 2)
	{
		return false;
	}

	//check for finalizations using playmode, floats translation is wrinte in comments
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

	//if none is true is not a finalization
	return false;
}

int Filter::lookForBegin(int i)
{
	//scope declaration for wich teams begins with the ball
	int teamOnBall = 0;

	//if its to early in the game it cant be a play
	if (i < 23 * 3)
	{
		return 0;
	}

	//check for wich teams was first in the ball
	for (int j = i - 23 * 3; j < i - 23 * 2; j++)
	{
		if (data[j][9])
			teamOnBall = data[j][1];
	}

	//checks for last deadBall or change in
	//ball posession
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

	//if neither a deadball or change in possession
	//was found returns maximum length possibel
	//max play length
	return data[i - MAX_PLAY_LENGTH * 23][0];
}

//SET FILTERED
void Filter::setFiltered()
{
	//filters data using plays
	//basically checks if lines in data
	//are within plays boundaries
	//when a line in data is over a play
	//final boundarie, it starts checking
	//with the next play, no need to
	//redo past data lines, since they are in
	//cycle order.
	unsigned int playIndex = 0;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i][0] >= plays[playIndex][0] and data[i][0] < plays[playIndex][1])
		{
			filtered.push_back(data[i]);
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

//SET PATHS
void Filter::createPaths()
{
	//clear path before loading
	paths.clear();
	path.clear();

	//scope declarations for a variable
	std::vector<int> playFirstLine;

	//defining the first data line that represents each
	//plays first cycle.
	playFirstLine.push_back(0);
	for (unsigned int i = 1; i < filtered.size(); i++)
	{
		if (filtered[i][0] - filtered[i - 1][0] > 1)
		{
			playFirstLine.push_back(i);
		}
	}

	//this for is quite mind bogging, but what it is doing is.
	//takes the first line in data that represents the begin of a play
	//iterating over playFirstLine.
	//play 1 -> cycle 660 -> line 10000, for example (example)
	//the uses a delta for player (0-22) to iterate insde said cycle
	//because cycle 660 is actualy composed of 23 lines in data
	//than using data delta iterates with a sum of 23 so that
	//it will take the next cycles for the same player.
	//so inside for for cycles, medium for for players, outside for for plays
	//it will outputs something like the following:

	// i = 1000 -> cycle 660 (begin of first PLay) -> player 0
	// i = 1000 + 23 -> cycle 661 -> player 0
	// ...
	// i = 1000 + 23 * inside for -> cycle 810 (end of first Play) -> player 0

	// i = 1000 + 1 -> cycle 660 (begin of first PLay) -> player 1
	// i = 1000 + 1 + + 23 -> cycle 661 -> player 1
	// ...
	// i = 1000 + 1 + (23 * inside for) -> cycle 810 (end of first Play) -> player 1

	// i = 2000 -> cycle 900 (begin of second PLay) -> player 0
	// i = 2000 + 23 -> cycle 661 -> player 0
	// ...
	// i = 2000 + 23 * inside for -> cycle 1050 (end of second Play) -> player 0

	// and so on
	for (unsigned int plf = 0; plf < playFirstLine.size(); plf++)
	{
		for (int player = 0; player < 23; player++)
		{
			path.push_back(filtered[player + playFirstLine[plf]][0]);
			path.push_back(filtered[player + playFirstLine[plf]][1]);
			path.push_back(filtered[player + playFirstLine[plf]][2]);
			unsigned int delta = player + playFirstLine[plf];
			for (unsigned int i = delta; i < filtered.size(); i = i + 23)
			{
				if ((i > delta + 23 and (filtered[i][0] - filtered[i - 23][0] != 1)))
				{
					paths.push_back(path);
					path.clear();
					break;
				}

				path.push_back(filtered[i][3]);
				path.push_back(filtered[i][4]);
				path.push_back(filtered[i][5]);
				path.push_back(filtered[i][6]);

				if ((i > filtered.size() - 24))
				{
					paths.push_back(path);
					path.clear();
					break;
				}
			}
		}
	}
}

//OUTPUT
void Filter::saveCsv()
{
	// check if dir exists and creates it if it doesnt.
	if (!std::filesystem::exists(filteredDir))
		std::filesystem::create_directories(filteredDir);

	//  open file
	std::fstream csvFile;
	csvFile.open(filteredPath, std::ios::out | std::ios::trunc);

	// write headers
	csvFile << "cycle, team, player, ";
	for (int i = 0; i < MAX_PLAY_LENGTH; i++)
	{
		csvFile << "pos_x_" << i << ", "
						<< "pos_y_" << i << ", speed_x_"
						<< i << ", speed_y_" << i;
		if (i != MAX_PLAY_LENGTH - 1)
			csvFile << ", ";
	}
	csvFile << std::endl;

	//print data
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		for (unsigned int j = 0; j < paths[i].size(); j++)
		{
			csvFile << paths[i][j];
			if (j != paths[i].size() - 1)
				csvFile << ", ";
		}
		csvFile << std::endl;
	}

	//close file
	csvFile.close();
	return;
}

void Filter::saveRaw(bool singleFile)
{
	// check if dir exists and creates it if it doesnt.
	if (!std::filesystem::exists(rawDir))
		std::filesystem::create_directories(rawDir);

	// open file
	if (singleFile)
	{
		rawPath = rawDir + "/rawOutput"
	}
	std::fstream rawFile;
	rawFile.open(rawPath, std::ios::out | std::ios::app);

	// print data
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		for (unsigned int j = 3; j < paths[i].size(); j++)
		{
			rawFile << paths[i][j];
			if (j != paths[i].size() - 1)
				rawFile << ", ";
		}
		rawFile << std::endl;
	}

	// close file
	rawFile.close();
	return;
}

//USAGE
void Filter::filterDir(const std::string &rootDir)
{
	//get list of all csv files in root dir
	std::vector<std::string> inputPaths = listFiles(rootDir, ".csv");

	//for each vsc file loads all data into class and outputs it
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
			std::cout << "\t" << i << ".4. loading plays." << std::endl;
			setPlays();
			std::cout << "\t" << i << ".5. filtering data." << std::endl;
			setFiltered();
			std::cout << "\t" << i << ".6. creating paths." << std::endl;
			createPaths();
			std::cout << "\t" << i << ".7. writing csv to I/O." << std::endl;
			saveCsv();
			std::cout << "\t" << i << ".8. writin raw to I/O" << std::endl;
			saveRaw();
		}
		catch (const std::exception &e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl;
			return;
		}
	}
}
