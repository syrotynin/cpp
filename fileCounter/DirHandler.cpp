#include "DirHandler.h"


DirHandler::DirHandler(){
	// all threads finished or escaped
	stop_condition = false;
}

void DirHandler::escape_handling(){

	while (true){
		if ((GetAsyncKeyState(VK_ESCAPE) != 0) || all_thr_fin())
			break;
	}

	stop_condition = true;
}


void DirHandler::setPaths(string in_file, string out_file)
{
	try{
		ifstream file(in_file);
		out_file_name = out_file;
		// parse pathes and create separate thread for each
		string line;
		if (file.is_open())
		{
			Lines = 0;

			while (getline(file, line))
			{
				//cout << line << endl;
				pair<string, int> dir = make_pair(line, 0);
				paths.push_back(dir);
				Lines++;
			}

			start_threads(Lines);

			file.close();
		}
		else
		{
			throw "Cannot open file!";
		}
	}
	catch (ifstream::failure e) {
		cout << "Exception opening/reading file " << e.what() << endl;
	}
}

void DirHandler::start_threads(int amount){
	// boolean array of running threads
	running_thr = new bool[Lines];

	// add thread for path
	for (int i = 0; i < Lines; i++){
		threads.emplace_back(&DirHandler::files_counting, this, &paths[i], paths[i].first);
		running_thr[i] = true;
	}

	// ESCaping thread
	thread thr(&DirHandler::escape_handling, this);
	thr.join();

	for (auto& thr : threads){
		thr.join();
	}
}

void DirHandler::files_counting(pair<string, int> * dir, string cur_path)
{
	
	if (stop_condition)
		return;

	path p(cur_path);

	try
	{
		if (exists(p))    // does p actually exist?
		{
			if (is_regular_file(p)){        // is p a regular file?  
				//cout << p << " - It's file. size is " << file_size(p) << endl;
			}

			else if (is_directory(p))      // is p a directory?
			{
				for (directory_iterator it(p); it != directory_iterator(); ++it)
				{
					if (is_regular_file(*it)){
						dir->second++;
					}
					else if (is_directory(*it)){
						// dive to next directory
						string next_path = it->path().string();
						files_counting(dir, next_path);
					}
				}
			}

			else{
				//cout << p << " exists, but is neither a regular file nor a directory" << endl;
			}
		}
		else
		{
			//cout << p << " does not exist" << endl;
		}
	}

	catch (const filesystem_error& ex)
	{
		cout << ex.what() << endl;
	}

	// thread is finishing
	// when path name equals initial directory name
	if (dir->first.compare(cur_path) == 0){
		thr_finishes();
	}
}

void DirHandler::thr_finishes(){
	for (int i = 0; i < Lines; i++){
		if (running_thr[i] == true){
			running_thr[i] = false;
			break;
		}
	}
}

bool DirHandler::all_thr_fin(){
	for (int i = 0; i < Lines; i++){
		if (running_thr[i] == true){
			return false;
		}
	}

	return true;
}

void DirHandler::output(){

	ofstream outfile;
	outfile.open(out_file_name, ios::out);

	int i = 1;
	for (auto p : paths){
		cout << i << ". " << p.second << " files, " << p.first << endl; // on screen
		outfile << p.first << ";" << p.second << endl;
		i++;
	}
}