#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <windows.h>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

class DirHandler{
	
public:
	DirHandler();

	void setPaths(string in_file, string out_file);
	void output();

private:

	vector<thread> threads; 
	bool* running_thr; // true if thread is running
	vector<pair<string, int>> paths; // current directory name and files amount

	void files_counting(pair<string, int> * dir, string cur_path); // (directory - amount of files) pair

	string out_file_name; // name of output file
	int Lines; // amount of initial directories

	void start_threads(int amount);
	void thr_finishes(); // thread finishes execution
	bool all_thr_fin(); // true if all threads finished counting


	void escape_handling();
	bool stop_condition; // condition when ESC pressed
};

