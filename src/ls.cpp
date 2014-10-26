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

	for (unsigned i = 0; i < argc; ++i) {				//fill in a vector with the passed in arguments
		arg_list.push_back(argv[i]);
	}

	for (unsigned i = 0; i < arg_list.size(); ++i) {			//distinguish between directories and arguments
		if (arg_list.at(i).at(0) != '-') { 
			string adirectory = arg_list.at(i);
			directories.push_back(adirectory);
		}
		
		else if (arg_list.at(i).at(0) == '-') {
			string anargument = arg_list.at(i);
			user_arg.push_back(anargument);
		}

	}


	char *dirName = ".";
	DIR *dirp = opendir(dirName)
	dirent *direntp;								
	while ((direntp = readdir(dirp)))	{ //readdir returns a pointer to the next directory 
										  //(which is a dirent struct that is pointed to by dirp)
		
		cout << direntp->d_name << endl; // use stat here to find attributes of file
	}
	closedir(dirp);
}


