//TO DO
//mkdir data before run
//files per team

#include "Reader.h"
#include "Filter.h"
#include "fileFormat.h"

int main(int argc, char *argv[])
{
	// main
	fileFormat format = fileFormatUserInstancer();

	//read rcg
	Reader reader;
	reader.readDir();

	//filter data
	Filter filter(format);
	filter.filterDir();
}
