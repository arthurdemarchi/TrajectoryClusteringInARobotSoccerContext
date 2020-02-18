#include "Reader.h"
#include "Filter.h"

int main(int argc, char *argv[])
{
	// main

	//read rcg
	Reader reader;
	reader.readDir();

	//filter data
	Filter filter;
	filter.filterDir();
}
