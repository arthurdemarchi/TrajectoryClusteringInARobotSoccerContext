#include "tools.h"

std::vector<std::string> listLeaves(std::string rootDir){
    std::vector<std::string> listOfLeaves;
    bool has_folders = false;
    for (const auto & entry : std::filesystem::recursive_directory_iterator(rootDir)){
        
        if (!std::filesystem::is_directory(entry.path()))
            continue;

        for (const auto & inner_entry : std::filesystem::directory_iterator(entry.path())){
            has_folders = false;
            if (std::filesystem::is_directory(inner_entry.path()))
                has_folders = true;
        }

        if(has_folders == true)
            continue;

        listOfLeaves.push_back(entry.path());
    }
    return listOfLeaves;
};