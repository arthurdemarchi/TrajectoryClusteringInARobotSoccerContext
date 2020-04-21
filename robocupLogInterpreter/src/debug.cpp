#include "debug.h"

// TODO(not required): change index to consant table
// void printPlays(Filter filter)
// {
//     for (unsigned int i = 0; i < filter.plays.size(); i++)
//         std::cout << "[DEBUG] play " << i << " goes from: " << filter.plays[i][0] << " to: " << filter.plays[i][1] << " and attacker is team " << filter.plays[i][2] << std::endl;
// }

// TODO(not required): Add other attributes of struct and a abrief inline optinon
void printFileFormat(fileFormat format)
{
    std::cout << "## FILE FORMAT STUCT ##" << std::endl;
    std::cout << "    Grouping by team: " << format.groupBy << std::endl;
    std::cout << "    Header: " << format.header << std::endl;
    std::cout << "    row ID: " << format.rowId << std::endl;
    std::cout << "    Play Length: " << format.playLength << std::endl;
    std::cout << "    Filtering type: " << format.filterBy << std::endl;
}