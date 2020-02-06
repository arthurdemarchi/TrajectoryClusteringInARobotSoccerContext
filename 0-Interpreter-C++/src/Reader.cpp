#include "Reader.h"

Reader::Reader(std::string gameDir)
{
    setPaths(gameDir);
    openFiles();
    
    // debug("Reader was created for a game in: '" << gameDir << "'.");
}

Reader::~Reader()
{
    // debug("Reader from game in: '" << gameDir << "' was destroyed.");
}

std::string Reader::getDefaultPath()
{
    std::string binPath = std::filesystem::current_path();
    binPath.replace(binPath.end()-3, binPath.end(), "data/");
    debug("The Default Data Path is: " << binPath);
    return binPath;
}

bool Reader::setPaths(std::string gameDir)
{
    
    if (!std::filesystem::is_directory(gameDir)){
        throw std::runtime_error("Invalid Directory");
    }
    
    rclPath = "";
    rcgPath = "";
    this->gameDir = gameDir;

    for (const auto & entry : std::filesystem::directory_iterator(gameDir)){
        std::string filePath = entry.path();
        std::string fileExtension = filePath.substr(filePath.size()-4, std::string::npos);
        if(fileExtension == ".rcg"){
            if(rcgPath == ""){
                rcgPath = filePath;
            }
            else{
                throw std::runtime_error("More than one .rcg file was found in the game directory.");
            }
        }
        if(fileExtension == ".rcl"){
            if(rclPath == ""){
                rclPath = filePath;
            }
            else{
                throw std::runtime_error("More than one .rcl file was found in the game directory.");
            }
        }
    }

    if(rcgPath == "" || rclPath == ""){
        throw std::runtime_error("No rcl or rcg file was found in the game directory.");
    }

    return true;
}

void Reader::openFiles()
{
    std::fstream rcgFile(rcgPath);
    std::fstream outputFile("/home/arthurdemarchi/teste.txt");
    std::string line, cycleLines[6000];
    int cycle = 0;
    while(getline(rcgFile, line))
    {
        if(line.find("(show ")!=std::string::npos){
            if(cycle > 1){
                if(cycleLines[cycle-1].substr(0, 4) == line.substr(6, 4)){
                    continue;
                }
            }
            cycleLines[cycle] = line.substr(6);
            cycle++;
        }
    }

    std::string patterns [23] = {
        "((b)", "((l 1)", "((l 2)", "((l 3)", "((l 4)",
        "((l 5)", "((l 6)", "((l 7)", "((l 8)", "((l 9)",
        "((l 10)", "((l 11)", "((r 1)", "((r 2)", "((r 3)",
        "((r 4)", "((r 5)", "((r 6)", "((r 7)", "((r 8)",
        "((r 9)", "((r 10)", "((r 11)"
    };

    int position;
    std::string streamString;
    std::string columns[5];
    std::string playerName;
    std::string playerLines[6000*23];
    for(int i = 0; i < 6000; i++){
        columns[0] = cycleLines[i].substr(0, cycleLines[i].find(" "));
        for(int j = 0; j < 23; j++){
            if(cycleLines[i].find(patterns[j]) == std::string::npos)
                continue;
            
            position = cycleLines[i].find(patterns[j]) + patterns[j].size() + 1;
            streamString = cycleLines[i].substr(position, 64);
            // debug(columns[0] << ": " << streamString);

            if(j!=0){
                position = streamString.find(" ") + 1;
                streamString = streamString.substr(position, 64);
                // debug("pre0: " << streamString);
               
                position = streamString.find(" ") + 1;
                streamString = streamString.substr(position, 64);
                // debug("pre1: " << streamString);
            }
            
            for(int c=1; c<5; c++){
                columns[c] = streamString.substr(0, streamString.find(" "));
                // debug(c << "th: " << columns[c]);

                position = streamString.find(" ") + 1;
                streamString = streamString.substr(position, 64);
                // debug("pre1: " << streamString);
            }
            
            if(j==0){
                playerName = "ball";
                columns[4] = columns[4].substr(0, columns[4].size()-1);
            }
            else if(j>11){
                playerName = "right " + std::to_string(j-11);
            }
            else{
                playerName = "left " + std::to_string(j);
            }

            playerLines[i*23+j] = columns[0] + ", " + playerName + ", " + columns[1] + ", " + columns[2] + ", " + columns[3] + ", " + columns[4];
            outputFile << playerLines[i*23+j] << std::endl;
        }    
    }
}