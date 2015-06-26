// fileCounter.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <windows.h>
#include "DirHandler.h"

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	DirHandler dirHandler = DirHandler();
	dirHandler.setPaths(argv[1], argv[2]);

	dirHandler.output();
	
	return 0;
}

