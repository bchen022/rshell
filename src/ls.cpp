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
#include <iomanip>

using namespace std;

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
	return toupper(a[j]) < toupper(b[k]);
}

int main(int argc, char* argv[]) {

	string ls_check = "ls";
	if (argv[1] != ls_check) {
		cout << "bash: " << argv[1] << ": command not found" << endl;
		exit(1);
	}
	
	char dot[2] ;
	strcpy(dot, ".");

	vector<string> arg_list;
	vector<string> directories;
	vector<string> user_arg;
	vector<string> files;
	int a_flag = 0;
	int l_flag = 0;
	int R_flag = 0;
	int x_flag = 0;

	for (unsigned i = 1; i < argc; ++i) {				//fill in a vector with the passed in arguments
		arg_list.push_back(argv[i]);
	}

	for (unsigned i = 1; i < arg_list.size(); ++i) {			//distinguish between directories and arguments
		if (arg_list.at(i).at(0) == '/') { 
			string adirectory = arg_list.at(i);
			directories.push_back(adirectory);
		}
		
		else if (arg_list.at(i).at(0) == '-') {
			string anargument = arg_list.at(i);
			user_arg.push_back(anargument);
		}
		
		else {
			string afile = arg_list.at(i);
			files.push_back(afile);
		}

	}

	if (directories.size() == 0) {
		directories.push_back(".");
	}
	
	for (unsigned i = 0; i < user_arg.size(); ++i) {
		if (user_arg.at(i).find('a') != std::string::npos) {
			++a_flag;
		}
		if (user_arg.at(i).find('l') != std::string::npos) {
			++l_flag;
		}
		if (user_arg.at(i).find('R') != std::string::npos) {
			++R_flag;
		}
	}

	int file_sz = files.size();


	for (unsigned i = 0; i <directories.size(); ++i) {
		cout << "directories: " << directories.at(i) << endl;
	}

	for (unsigned i = 0; i <user_arg.size(); ++i) {
		cout << "user arg: " << user_arg.at(i) << endl;
	}
	
	for (unsigned i = 0; i <files.size(); ++i) {
		cout << "files: " << files.at(i) << endl;
	}

	cout << "-------------------------------------" << endl;
	cout << endl;

	while (file_sz > 0) {

		struct stat statbuf;
		if (-1 == stat(files.at(file_sz -1).c_str(), &statbuf)) {
			perror("Could not stat the file." );
			exit(1);
		}

		if (l_flag > 0) {
		
			if (S_ISDIR(statbuf.st_mode)) {
				cout << "d";
			}
			else {
				if(S_IXUSR & statbuf.st_mode) {
					x_flag++;
				}
	 			if (S_IXGRP & statbuf.st_mode) {
					x_flag++;
				}
				if (S_IXOTH & statbuf.st_mode) {
					x_flag++;
				}	
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
			timeinfo = localtime(&statbuf.st_mtime);
			if (strftime(time_buffer, 80, "%b %d %H:%M", timeinfo) != 0) {
				cout << time_buffer << " ";
			}
			else {
				perror("Unable to retreive modification time." );
			}
			
			if (x_flag > 0) {
				cout << files.at(file_sz-1) << "* "  << endl;
				x_flag = 0;
			}
			
			else {
				cout << files.at(file_sz-1) << endl;
			}

			file_sz--;
		}

		else {
			cout << files.at(file_sz -1) << "  ";
		
			file_sz--;
			if (file_sz == 0){
				cout << endl;			
			}
		}
	}

	if (files.size() > 0 ) {
		if (directories.at(0).c_str()[0] == '/') {
			cout << endl;
			cout << directories.at(0) << ": " << endl;
		}
		if (directories.size() == 1 && directories.at(0).c_str()[0] != '/') {
			exit(1);
		}	
	}

	unsigned initial_dir_sz = directories.size();
	while (!directories.empty() ) {
		string current_dir = directories.back();
		if (initial_dir_sz > 1) {
			cout << "current_dir: " << current_dir << endl;
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
		
		unsigned sorted_sz = sort_dir2.size();
		vector<char*> sort_dir;
		for (unsigned i = 0; i < sorted_sz; ++i) {
			sort_dir.push_back(sort_dir2.back());
			sort_dir2.pop_back();
		}

		if (l_flag > 0) {
			unsigned i = 0;	
			unsigned total_block_temp = 0;
			unsigned total_block = 0;
			while(i < sorted_sz) {	
								
				if (a_flag > 0) {
					string entire_path = current_dir;
					entire_path.append("/");
					string file_name = sort_dir.at(i);
					entire_path.append(file_name);
					struct stat statbuf;
		
					if (-1 == stat(entire_path.c_str(), &statbuf)) {
						perror("Could not stat the directory.");
						exit(1);
					}			
					total_block_temp = statbuf.st_blocks / 2;
					total_block = total_block + total_block_temp;
					++i;
					//the total disk allocation for all files in that directory
				}
				
				else {
					string entire_path = current_dir;
					entire_path.append("/");
					string file_name = sort_dir.at(i);
					if (sort_dir.at(i)[0] == dot[0]) {
						++i;
						continue;
					}

					entire_path.append(file_name);
					struct stat statbuf;
		
					if (-1 == stat(entire_path.c_str(), &statbuf)) {
						perror("Could not stat the directory.");
						exit(1);
					}			
					total_block_temp = statbuf.st_blocks / 2;
					total_block = total_block + total_block_temp;
					++i;			
				}
			}
			cout << "total " << total_block << endl;
		}

/*		unsigned longest_name = 0;
		for (unsigned i = 0; i < sort_dir.size(); ++i) {
			unsigned longest_name_temp = 0;
			unsigned j = 0;
			while (sort_dir.at(i)[j] != '\0') {
				longest_name_temp++;
				++j;
			}
			if (longest_name_temp > longest_name) {
				longest_name = longest_name_temp;					
			}
		}
		
		cout << "longest_name: " << longest_name << endl;
*/		
		while (sorted_sz > 0) {
			string entire_path = current_dir;
			entire_path.append("/");
			string file_name = sort_dir.at(sorted_sz-1);
			entire_path.append(file_name);
			
			struct stat statbuf;
			if (-1 == stat(entire_path.c_str(), &statbuf)) {
				perror("Could not stat the directory.");
				exit(1);
			}
	
			if (R_flag > 0) {
				if (statbuf.st_mode & S_IFDIR) {
					cout << "current_dir: " << current_dir << ": " << endl;
					char entire_path2[entire_path.length()];
					strcpy(entire_path2, entire_path.c_str());
					sort_dir.push_back(entire_path2);
	//				sorted_sz++;
				}
			}
	
	
			if (l_flag > 0) {
			
				if (sort_dir.at(sorted_sz-1)[0] == '.' && a_flag == 0) {
					sorted_sz--;
					continue; 
				}
				
				if (S_ISDIR(statbuf.st_mode)) {
					cout << "d";
				}
				else {
					if(S_IXUSR & statbuf.st_mode) {
						x_flag++;
					}
			 		if (S_IXGRP & statbuf.st_mode) {
						x_flag++;
					}
					if (S_IXOTH & statbuf.st_mode) {
						x_flag++;
					}	
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
				timeinfo = localtime(&statbuf.st_mtime);
				if (strftime(time_buffer, 80, "%b %d %H:%M", timeinfo) != 0) {
					cout << time_buffer << " ";
				}
				else {
					perror("Unable to retreive modification time." );
				}
				
				if (x_flag > 0) {
					cout << sort_dir.at(sorted_sz-1) << "* "  << endl;
					x_flag = 0;
				}
				
				else {
					cout << sort_dir.at(sorted_sz-1) << endl;
				}
			}
			
			else {
	
				if (!S_ISDIR(statbuf.st_mode)) {
					if(S_IXUSR & statbuf.st_mode) {
						x_flag++;
					}
					if (S_IXGRP & statbuf.st_mode) {
						x_flag++;
					}
					if (S_IXOTH & statbuf.st_mode) {
						x_flag++;
					}	
				}
			
				if (sort_dir.at(sorted_sz-1)[0] == '.' && a_flag == 0) {
					sorted_sz--;
					continue;
				}
	
				if (x_flag > 0) {
//					cout << left << setw(longest_name) << sort_dir.at(sorted_sz-1) << left << "*  ";
					cout << sort_dir.at(sorted_sz-1) << "*  ";
					x_flag = 0;
				}
	
				else {
//					cout << left << setw(longest_name) << sort_dir.at(sorted_sz-1) << left <<  "  ";
					cout << sort_dir.at(sorted_sz-1) << "  ";
				}
			}
			
			sorted_sz--;
		}

		if (l_flag > 0) {
		}
		else {
			cout << endl;	
		}
		closedir(dirp);
		directories.pop_back();
		if (initial_dir_sz > 1) {
			cout << endl;
		}
	}
}
