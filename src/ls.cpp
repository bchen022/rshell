#include <sys/types.h>		//stat, closedir, opendir
#include <dirent.h>			//readdir, closedir, opendir
#include <errno.h>			//perror
#include <stdio.h>			//exit, perror
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>			// stat
#include <sys/stat.h>		// stat
#include <cstdlib>
#include <pwd.h>			//userid
#include <grp.h>			//groupid
#include <time.h>			//time id
#include <algorithm>
#include <string.h>

using namespace std;

/* This is a BARE BONES example of how to use opendir/readdir/closedir. Notice
that there is no error checking on these functions. You MUST add error
 checking yourself.
*/

bool compare_dname(char* a, char* b) {

	unsigned j = 0;
	unsigned k = 0;
	char dot[2] ;
	strcpy(dot, ".");

	if (a[j] == dot[0]) {
		while (a[j] == dot[0]) {
			j++;
		}
	}

	if (b[k] == dot[0]) {
		while(b[k] == dot[0]) {
			k++;
		}
	}

//	cout << "cock" << endl;

	return toupper(a[j]) < toupper(b[k]);


}

int main(int argc, char* argv[]) {

	string ls_check = "ls";
	if (argv[1] != ls_check) {
//		cout << "not ls" << endl;
		cout << "bash: " << argv[1] << ": command not found" << endl;
		exit(1);
	}

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

	if (directories.size() == 0) {
		directories.push_back(".");
	}
	
	for (unsigned i = 0; i < user_arg.size(); ++i) {
		if (user_arg.at(i).find('a') != std::string::npos) {
//			cout << "a_flag found." << endl;
			++a_flag;
		}
		if (user_arg.at(i).find('l') != std::string::npos) {
//			cout << "l_flag found." << endl;
			++l_flag;
		}
		if (user_arg.at(i).find('R') != std::string::npos) {
//			cout << "R_flag found." << endl;
			++R_flag;
		}
	}

	struct stat statbuf;
	string current_dir = directories.back();
	if (-1 == stat(current_dir.c_str(), &statbuf)) {
		perror("Could not stat the directory.");
		exit(1);
	}
	const char *dirName = directories.back().c_str();
	DIR *dirp = opendir(dirName);			//opens and returns a directory stream //dirp points to the directory stream
	if (dirp == NULL) {
		perror("Could not open directory stream.");
		exit(1);	
	}
	dirent *direntp;
	vector<char*> sort_dir2;
	char slash[2];
	strcpy(slash, "/");
	while ((direntp = readdir(dirp))) {
		if (DT_DIR == (direntp->d_type)) {
			strncat(direntp->d_name, slash, 1 );
		}
		sort_dir2.push_back(direntp->d_name);
	}
	sort(sort_dir2.begin(), sort_dir2.end(), compare_dname);
//	for (unsigned i = 0; i < sort_dir.size(); ++i) {
//		cout << "sorted dir: " << sort_dir.at(i) << endl;	
//	}
	
	unsigned sorted_sz = sort_dir2.size();
	vector<char*> sort_dir;
	for (unsigned i = 0; i < sorted_sz; ++i) {
		sort_dir.push_back(sort_dir2.back());
		sort_dir2.pop_back();
	}
	
//	cout << "sorted_sz: " << sorted_sz << endl;
	while (sorted_sz > 0) {
	//while ((direntp = readdir(dirp)))	{ //readdir returns a pointer to a dirent struct from dirp (the directory stream)
										  //while direntp is a dirent struct from the directory stream
	//	cout << direntp->d_name << endl; // use stat here to find attributes of file
										 // int stat(const char* path, struct stat* buf) 
										 // stats the file pointed to by path and fills in buf with a ptr to the stat file
		string entire_path = current_dir;
		entire_path.append("/");
	//	string file_name = direntp->d_name;
		string file_name = sort_dir.at(sorted_sz-1);
		entire_path.append(file_name);
		
//		cout << "entire path: " << entire_path << endl;
		struct stat statbuf;
		if (-1 == stat(entire_path.c_str(), &statbuf)) {
			perror("Could not stat the directory.");
			exit(1);
		}
		
		//cout << "current directory:" << current_dir << endl;
		if (l_flag > 0) {

		/*	if (direntp->d_name[0] == '.' && a_flag == 0) {
				continue;
			}
		*/

//			cout << "AAAAAAAAAAAAA" << endl;
			if (sort_dir.at(sorted_sz-1)[0] == '.' && a_flag == 0) {
//				cout << "stuck here" << endl;
				sorted_sz--;
				continue; //infinite loop here fuckckckckcfkck c
			}
//			cout << "BBBBBBBBBBBBBB" << endl;

			if (S_ISDIR(statbuf.st_mode)) {
				cout << "d";
			}//
			else {
				cout << "-";
			}
		
			if (S_IRUSR & statbuf.st_mode) {
				cout << "r";	
			}
			else {
				cout << "-";
			}

			if(S_IWUSR & statbuf.st_mode) {
				cout << "w";
			}
			else {
				cout << "-";	
			}

			if(S_IXUSR & statbuf.st_mode) {
				cout << "x";
			}
			else {
				cout << "-";
			}

			if (S_IRGRP & statbuf.st_mode) {
				cout << "r";
			}
			else {
				cout << "-";
			}

			if (S_IWGRP & statbuf.st_mode) {
				cout << "w";
			}
			else {
				cout << "-";
			}

			if (S_IXGRP & statbuf.st_mode) {
				cout << "x";
			}
			else {
				cout << "-";
			}

			if (S_IROTH & statbuf.st_mode) {
				cout << "r";
			}
			else {
				cout << "-";
			}

			if (S_IWOTH & statbuf.st_mode) {
				cout << "w";
			}
			else {
				cout << "-";
			}

			if (S_IXOTH & statbuf.st_mode) {
				cout << "x";
			}
			else {
				cout << "-";
			}

			cout << " " << statbuf.st_nlink << " ";
			struct passwd *pw;
			if ((pw = getpwuid(statbuf.st_uid)) != NULL) {
				cout << pw->pw_name << " ";
			}
			else {
				perror("Unable to retreive user id.");
			}
			struct group *gr;
			if ((gr = getgrgid(statbuf.st_gid)) != NULL) {
				cout << gr->gr_name << " ";
			}
			else {
				perror("Unable to retreive group id.");
			}
			
			cout << statbuf.st_size << " ";
			
			struct tm* timeinfo;
			char time_buffer [80];
			//time(&statbuf.st_mtime);
			timeinfo = localtime(&statbuf.st_mtime);
			if (strftime(time_buffer, 80, "%b %d %H:%M", timeinfo) != 0) {
				cout << time_buffer << " ";
			}
			else {
				perror("Unable to retreive modification time." );
			}
			
	/*		if (DT_DIR == (direntp->d_type)) {
				cout << direntp->d_name << "/" << endl;
			}
	*/
	//		else {
	//			cout << direntp->d_name << endl;
	//		}
			
			cout << sort_dir.at(sorted_sz-1) << endl;
		}
		

		else {
			//cout << "no -l flag" << endl;
	/*		if (direntp->d_name[0] == '.' && a_flag == 0) {
				continue;
			}

			cout << direntp->d_name << "  ";		
		}
	*/		
			
	//		cout << "CCCCCCCCC" << endl;
			
			if (sort_dir.at(sorted_sz-1)[0] == '.' && a_flag == 0) {
				sorted_sz--;
				continue;
			}

			cout << sort_dir.at(sorted_sz-1) << "  ";

		}
		
		sorted_sz--;
	}

	cout << endl;	
	closedir(dirp);
	cout << "closed the directory stream pointed to by dirp" << endl;
	directories.pop_back();

//		exit(1);				//
}


