#include "debug.h"
#include "Reader.h"
#include "tools.h"

int main(int argc, char *argv[])
{
    std::vector<std::string> listOfLeaves;
    listOfLeaves = listLeaves("../data");

    for(unsigned int i=0; i<listOfLeaves.size(); ++i){
        try{
            Reader reader(listOfLeaves[i]);
        }
        catch(const std::exception & e){
            std::cerr << "ERROR: " << e.what() << std::endl;
            return 0;
        }
    }
    std::cout << "cabei" << std::endl;
}