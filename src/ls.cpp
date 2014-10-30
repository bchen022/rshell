#include <sys/types.h>		//stat, closedir, opendir
#include <dirent.h>			//readdir, closedir, opendir
#include <errno.h>			//perror
#include <stdio.h>			//exit, perror
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>			// stat
#include <sys/stat.h>		// stat
using namespace std;

/* This is a BARE BONES example of how to use opendir/readdir/closedir. Notice
that there is no error checking on these functions. You MUST add error
 checking yourself.
*/

int main(int argc, char* argv[]) {

	vector<string> arg_list;
	vector<string> directories;
	vector<string> user_arg;
	int a_flag = 0;
	int l_flag = 0;
	int R_flag = 0;

	for (unsigned i = 1; i < argc; ++i) {				//fill in a vector with the passed in arguments
		arg_list.push_back(argv[i]);
	}

	for (unsigned i = 1; i < arg_list.size(); ++i) {			//distinguish between directories and arguments
		if (arg_list.at(i).at(0) != '-') { 
			string adirectory = arg_list.at(i);
			directories.push_back(adirectory);
		}
		
		else if (arg_list.at(i).at(0) == '-') {
			string anargument = arg_list.at(i);
			user_arg.push_back(anargument);
		}

	}

	for (unsigned i = 0; i < arg_list.size(); ++i) {
		cout << "contents of arg_list: " << arg_list.at(i) << endl;
	}

	for (unsigned i = 0; i < directories.size(); ++i) {
		cout << "contents of directories: " << directories.at(i) << endl;
	}
	
	for (unsigned i = 0; i < user_arg.size(); ++i) {
		if (user_arg.at(i).find('a') != std::string::npos) {
			cout << "a_flag found." << endl;
			++a_flag;
		}
		if (user_arg.at(i).find('l') != std::string::npos) {
			cout << "l_flag found." << endl;
			++l_flag;
		}
		if (user_arg.at(i).find('R') != std::string::npos) {
			cout << "R_flag found." << endl;
			++R_flag;
		}
	}


	

/*
	char *dirName = ".";
	DIR *dirp = opendir(dirName);			//opens and returns a directory stream //dirp points to the directory stream
	if (dirp == NULL) {
		perror("Could not open directory stream.");
	}
	dirent *direntp;								
	while ((direntp = readdir(dirp)))	{ //readdir returns a pointer to the next directory from the directory stream
										  //(which is a dirent struct that is pointed to by dirp) //direntp points to next directory
		
		cout << direntp->d_name << endl; // use stat here to find attributes of file
										 // int stat(const char* path, struct stat* buf) 
										 // stats the file pointed to by path and fills in buf with a ptr to the stat file
	
		struct stat statbuf;
		if (-1 == stat(direntp->dname, &statbuf)) {
			perror("Could not stat the directory.");
		}
		else {
			
		}
		

	}
	closedir(dirp);
*/
}


