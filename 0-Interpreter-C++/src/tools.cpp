#include "tools.h"

std::vector<std::string> listLeaves(std::string rootDir)
{
	std::vector<std::string> listOfLeaves;
	std::string rcgPath;
	for (const auto &entry : std::filesystem::recursive_directory_iterator(rootDir))
	{

		if (std::filesystem::is_directory(entry.path()))
			continue;

		rcgPath = entry.path();
		if (!(rcgPath.rfind(".rcg") == rcgPath.size() - 4))
			continue;

		listOfLeaves.push_back(entry.path());
	}
	return listOfLeaves;
};
