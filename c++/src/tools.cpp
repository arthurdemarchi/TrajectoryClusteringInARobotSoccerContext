#include "tools.h"

std::vector<std::string> listFiles(std::string rootDir, std::string extension)
{
	std::vector<std::string> listOfRcgs;
	std::string entryPath;
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