#include "tools.h"

std::vector<std::string> listFiles(const std::string &rootDir, const std::string &extension)
{
	// scope declarations
	std::vector<std::string> listOfRcgs;
	std::string entryPath;

	//recursively lis all paths in a directory
	//for each entry(paths) checks if its a file or direcotry
	//and checks file extension.
	for (const auto &entry : std::filesystem::recursive_directory_iterator(rootDir))
	{
		if (std::filesystem::is_directory(entry.path()))
			continue;

		entryPath = entry.path();
		if (!(entryPath.rfind(extension) == entryPath.size() - 4))
			continue;

		listOfRcgs.push_back(entry.path());
	}
	return listOfRcgs;
}

bool booleanQuestion(const std::string &question)
{
	char yesOrNo;
	std::cout << question << std::endl;
	std::cout << "  y/n > ";
	do
	{
		scanf(" %c", &yesOrNo);
		getchar();
		if (yesOrNo == 'y')
		{
			return true;
		}
		else if (yesOrNo == 'n')
		{
			return false;
		}
		else
		{
			std::cout << "  Invalid option try again: " << std::endl;
			std::cout << "  y/n > ";
		}
	} while (true);
}

int multipleAlternativeQuestion(const std::string &question, std::vector<std::string> alternatives)
{
	unsigned int option;

	std::cout << question << std::endl;
	for (unsigned int i = 0; i < alternatives.size(); i++)
		std::cout << "  " << i << ") " << alternatives[i] << std::endl;

	std::cout << "    ";
	for (unsigned int i = 0; i < alternatives.size() - 1; i++)
		std::cout << i << "/";
	std::cout << alternatives.size() - 1 << " >";

	do
	{
		scanf(" %d", &option);
		getchar();
		if (option >= 0 and option < alternatives.size())
		{
			return option;
		}
		else
		{
			std::cout << "  Invalid option try again: " << std::endl;
			std::cout << "    ";
			for (unsigned int i = 0; i < alternatives.size() - 1; i++)
				std::cout << i << "/";
			std::cout << alternatives.size() - 1 << " >";
		}

	} while (true);
}
