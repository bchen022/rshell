#include <iostream>
#include <sys/types.h>		//fork, waitpid
#include <unistd.h>			//fork, execvp
#include <sys/wait.h>		//waitpid
#include <errno.h>			//perror
#include <stdio.h>			//perror, exit 
#include <boost/tokenizer.hpp>	//for parsing
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <stdlib.h>
using namespace std;
using namespace boost;

int main() {
	
	int pid = fork();	

	if (pid == -1) {	
		perror("The syscall fork() was unable to create a child process.");
		exit(1);
	}

	if (pid == 0) {	
		cout << "Entered the child process." << endl;

		while(1) {
			string userid = getlogin();
			if (NULL == getlogin()) {
				perror("Getlogin() could not retreive a username.");
			}
			char hostname[256];
			if (-1 == gethostname(hostname, sizeof(hostname))) {
				perror("Gethostname() could not retreive a hostname.");
			}
			
			cout << userid << "@" << hostname << "$ ";

			string commands;
			int and_flag = 0;
			int or_flag = 0;
			getline(cin, commands);														//get user input
			char* saveptr1;
			char* saveptr2;
			char* c_commands = new char [commands.length() + 1];						//create a c-string for strtok's argument
			strcpy(c_commands, commands.c_str());										//fill in the c-string with user input
			for (unsigned cmtfind = 0; cmtfind < strlen(c_commands); ++cmtfind) {
				if (c_commands[cmtfind] == '#') {
					c_commands[cmtfind] = '\0';
				}
				if (c_commands[cmtfind] == '&') {
					if (c_commands[cmtfind + 1] == '&') {
						cout << "and_flag was incremented. " << endl;
						and_flag = 1;
					}				
				}
				if (c_commands[cmtfind] == '|') {
					if (c_commands[cmtfind + 1] == '|') {
						cout << "or_flag was incremented. " << endl;
						or_flag = 1;
					}
				}

			}
	
			char delim_list[] = ";||&&";
			char* argv_tok = strtok_r(c_commands, delim_list, &saveptr1);					//parse the c-string(user input)

			vector<char*> argv_vect;
			vector<char*> argv_vect2;
			int i = 0;
			while (argv_tok != NULL) {													//fill vectors with parsed user input
				argv_vect.push_back(argv_tok);
				if (argv_vect.size() > 0) {
					for (unsigned argv_ctr = 0; argv_ctr < argv_vect.size(); ++argv_ctr) {
						cout << "contents of argv_vect1: " << argv_vect.at(argv_ctr) << endl;
					}

				}
				
				char* argv_tok2 = strtok_r(argv_vect.at(i), " " , &saveptr2);
				while (argv_tok2 != NULL) {
					argv_vect2.push_back(argv_tok2);
					argv_tok2 = strtok_r(NULL, " ", &saveptr2);
				}

				int command_sz = argv_vect2.size();
				char** argv = new char*[command_sz + 1]; 								//create argv for execvp
				for (unsigned j = 0; j < argv_vect2.size(); ++j) {
					argv[j] = new char[sizeof(argv_vect2.at(j)) + 1];					//filling in each index of argv with
					strcpy(argv[j], argv_vect2.at(j));									//enough space for cstring commands
				}
				for (unsigned argv2_ctr = 0; argv2_ctr < argv_vect2.size(); ++argv2_ctr) {
					cout << "contents of argv_vect2: " << argv_vect2.at(argv2_ctr) << endl;
				}
				for (unsigned argv3_ctr = 0; argv3_ctr < argv_vect2.size(); ++argv3_ctr) {
					cout << "contents of argv array: " << argv[argv3_ctr] << endl;
				}
			
				argv[argv_vect2.size()] = NULL;

				string exitor = "exit";
				string exitor2 = " exit";
				for (unsigned m = 0; m < argv_vect.size(); ++m) {
					cout << "entered loop. " << endl;
					int exit_comp = strcmp(argv_vect.at(m), exitor.c_str());
					int exit_comp2 = strcmp(argv_vect.at(m), exitor2.c_str());
					if (exit_comp == 0 || exit_comp2 == 0) {
						cout << "exited" << endl;
						exit(1);
					}
				}
				cout << "finished loop. " << endl;	
				int pid2 = fork();
				cout << "pid 2: " << pid2 << endl;
				int status = 0;
				if (pid2 == -1) {
					perror("Unable to fork for execvp.");
					exit(1);
				}
				if (pid2 == 0) {
					cout << "entered child branch." << endl;
					cout << "OR_FLAG: " << or_flag << endl;
					cout << "AND_FLAG: " << and_flag << endl;	
					
					/*if (or_flag > 0) {
						


					}*/
				
					if (-1 == execvp(argv[0], argv)) {
						perror("There was an error in execvp. ");
						if (and_flag > 0) {
							status = 3;
							cout << "should exit because AND was found. " << endl;
						//	exit(1);
							break;
						}

						if (or_flag > 0) {
							status = 3;
							cout << "should continue because OR was found2. " << endl;
							exit(1);
						//	shouldn't this mean the next command will work?
						//	break;
						}
						exit(1);
					}
					else { 
						if (or_flag > 0) {
							status = 3;
							cout << "should exit because OR was found. " << endl;
							break;
						}
					}
				}
				if (pid2 > 0) {
					cout << "entered parent branch. " << endl;
					if (-1 == wait(&status)) {
						perror("Still waiting for child execvp process to end.");
					}
				}
				
				argv_tok = strtok_r(NULL, delim_list, &saveptr1);

				++i;
				argv_vect2.clear();
			}
		}
	}

	else if (pid > 0) {
		//parent
		if (-1 == wait(0)) {		//wait returns PID of child (-1 if theres an error)
			perror("You are still waiting for the child process to end.");	
		}
	}
}

