# What is This?

A interpreter and filtering tool that provides more readability for the logs in roboup 2D Soccer simulation archive.
Besides making csv readable files from the logs the software can filter data providing a easy way of accessing
trajectories of each player in important finalization plays.

Has a compability mode to generate files in the exact manner the "traclus" algorithm, from another author, asks
and has a ball possession algortihm that was empirically tested.

## Instalation
No Instalation needed

## Requirements
* gcc compatible with c++17
* cmake 3.5 or newer

## Usage
After downloading it to local

1. Compile the program using
	 
        sh ./build.sh

3. Place log data into ./data/input:
        
        mv -r <YOUR/DATA/FOLDER/PATH> ./data/input

    or download directly from robocup archive and make them rcg files (robocup is now provind them as .gz for some reason)
        
        cd data/input
        wget -r -np -nH --no-check-certificate -R  index.html https://archive.robocup.info/Soccer/Simulation/2D/logs/RoboCup/2018/Elimination/
        mv Soccer/Simulation/2D/logs/RoboCup/2018/Elimination/ .
        rm -r Soccer/
        find . -name "*.gz" -exec bash -c 'mv "$1" "${1%.gz}"' - '{}' \;
        find . -name "*.rcl" -type f -delete
        find . -name "*.tmp" -type f -delete


4. Execute the program:

        cd ./bin
        ./app.bin

5. To stop the execution of the program press Ctrl+c.

6. To run again rename or delete the directory ./data/output.
	 
    