#include "tools.h"

// #listFiles
std::vector<std::string> listFiles(const std::string &rootDir, const std::string &extension)
{
	// scope declarations
	std::vector<std::string> listOfRcgs;
	std::string entryPath;

	//recursively lis all paths in a directory
	for (const auto &entry : std::filesystem::recursive_directory_iterator(rootDir))
	{
		//for each entry(paths) checks if its a file or direcotry
		if (std::filesystem::is_directory(entry.path()))
			continue;

		//for each entry(path) checks file extension.
		entryPath = entry.path();
		if (!(entryPath.rfind(extension) == entryPath.size() - 4))
			continue;

		//if it is not a directory and it has the right file extension append it to the lists
		listOfRcgs.push_back(entry.path());
	}

	//returns populated list
	return listOfRcgs;
}

// #booleanQuestion
bool booleanQuestion(const std::string &question)
{
	//declrare scope variables for user choice
	char yesOrNo;

	//prints question to terminal
	std::cout << question << std::endl;
	std::cout << "  y/n > ";
	do //until return
	{
		//get user input
		scanf(" %c", &yesOrNo);
		getchar();

		//if yes return true
		if (yesOrNo == 'y')
			return true;

		//if no return false
		else if (yesOrNo == 'n')
			return false;

		//else keep trying
		else
		{
			std::cout << "  Invalid option try again: " << std::endl;
			std::cout << "  y/n > ";
		}
	} while (true);
}

// #multipleAlternativeQuestion
int multipleAlternativeQuestion(const std::string &question, std::vector<std::string> alternatives)
{
	//declrare scope variables for user choice
	unsigned int option;

	// prints alternatives to terminal
	std::cout << question << std::endl;
	for (unsigned int i = 0; i < alternatives.size(); i++)
		std::cout << "  " << i << ") " << alternatives[i] << std::endl;

	//prints input choices to terminal
	std::cout << "    ";
	for (unsigned int i = 0; i < alternatives.size() - 1; i++)
		std::cout << i << "/";
	std::cout << alternatives.size() - 1 << " >";

	do // until return
	{
		//get user input
		scanf(" %d", &option);
		getchar();

		//if a valid input returns it
		if (option >= 0 and option < alternatives.size())
			return option;

		//else keep trying
		else
		{
			//invalid choice print
			std::cout << "  Invalid option try again: " << std::endl;
			//input choices print
			std::cout << "    ";
			for (unsigned int i = 0; i < alternatives.size() - 1; i++)
				std::cout << i << "/";
			std::cout << alternatives.size() - 1 << " >";
		}

	} while (true);
}
