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

	if (directories.size() == 0) {
		directories.push_back(".");
	}

/*	for (unsigned i = 0; i < arg_list.size(); ++i) {
		cout << "contents of arg_list: " << arg_list.at(i) << endl;
	}

	cout << "--------------------------------------------" << endl;

	for (unsigned i = 0; i < user_arg.size(); ++i) {
		cout << "contents of user_arg: " << user_arg.at(i) << endl;
	}

	for (unsigned i = 0; i < directories.size(); ++i) {
		cout << "contents of directories: " << directories.at(i) << endl;
	}
*/	
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

	struct stat statbuf;
	//!!!!!!!!!!this lists the directory's info  but its supposed to GO INTO the dir and list its contents' info
	//			./a.out ls testdir
	//			./a.out ls -l testdir and ./a.out ls testdir -l
	while (!directories.empty()) {
		string current_dir = directories.back();
		if (-1 == stat(current_dir.c_str(), &statbuf)) {
			perror("Could not stat the directory.");
			exit(1);
		}
		//cout << current_dir: << ": " << endl; 
		const char *dirName = directories.back().c_str();
		DIR *dirp = opendir(dirName);			//opens and returns a directory stream //dirp points to the directory stream
		if (dirp == NULL) {
			perror("Could not open directory stream.");
			exit(1);	
		}
		dirent *direntp;								
		while ((direntp = readdir(dirp)))	{ //readdir returns a pointer to the next directory from the directory stream
											  //(which is a dirent struct that is pointed to by dirp) //direntp points to next directory
			
		//	cout << direntp->d_name << endl; // use stat here to find attributes of file
											 // int stat(const char* path, struct stat* buf) 
											 // stats the file pointed to by path and fills in buf with a ptr to the stat file
			string entire_path = current_dir;
			entire_path.append("/");
			string file_name = direntp->d_name;
			entire_path.append(file_name);
		
			struct stat statbuf;
			if (-1 == stat(entire_path.c_str(), &statbuf)) {
				perror("Could not stat the directory.");
				exit(1);
			}
			
			//cout << "current directory:" << current_dir << endl;
			//i am statting current dir, should be statting the files inside the directories
			if (l_flag > 0) {

				if (direntp->d_name[0] == '.' && a_flag == 0) {
					continue;
				}

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
				time(&statbuf.st_mtime);
				timeinfo = localtime(&(statbuf.st_mtime));
				if (strftime(time_buffer, 80, "%b %d %H:%M", timeinfo) != 0) {
					cout << time_buffer << " ";
				}
				else {
					perror("Unable to retreive modification time." );
				}
				cout << direntp->d_name << endl;
			}
	
			else {
				//cout << "no -l flag" << endl;
				if (direntp->d_name[0] == '.' && a_flag == 0) {
					continue;
				}

				cout << direntp->d_name << " ";//if there was no l flag		
			}
		
			//cout << endl;
	
		}
		cout << endl;	
		closedir(dirp);
		cout << "closed the directory stream pointed to by dirp" << endl;
		directories.pop_back();

		exit(1);				//
	}
}


