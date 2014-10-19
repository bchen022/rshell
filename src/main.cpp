#include <iostream>
#include <sys/types.h>		//fork, waitpid
#include <unistd.h>			//fork, execvp
#include <sys/wait.h>		//waitpid
#include <errno.h>			//perror
#include <stdio.h>			//perror, exit (same as iostream?)
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
			string commands;
			string userid = getlogin();
			if (NULL == getlogin()) {
				perror("Getlogin() could not retreive a username.");
			}
			char hostname[256];
			if (-1 == gethostname(hostname, sizeof(hostname))) {
				perror("Gethostname() could not retreive a hostname.");
			}
			
			cout << userid << "@" << hostname << "$ ";

			
			//vector<vector<string>> true_argv_vect;
			getline(cin, commands);									//get user input
			if (commands == "exit") {
				exit(1);	
			}

			char* saveptr1;
			char* saveptr2;
			char* c_commands = new char [commands.length() + 1];	//create a c-string for strtok's argument/parameter
			strcpy(c_commands, commands.c_str());					//fill in the c-string with user input
			char* argv_tok = strtok_r(c_commands,";", &saveptr1);			//parse the c-string(user input)
		
			vector<char*> argv_vect;
			vector<char*> argv_vect2;
		
			int i = 0;
			//	cout << "argv_tok before: " << argv_tok << endl;
			while (argv_tok != NULL) {								//fill vector with parsed user input
				argv_vect.push_back(argv_tok);
				char* argv_tok2 = strtok_r(argv_vect.at(i), " ", &saveptr2);
				while (argv_tok2 != NULL) {
					argv_vect2.push_back(argv_tok2);
					argv_tok2 = strtok_r(NULL, " ", &saveptr2);
				}
				
				//cout << "argv_tok again: " << argv_tok << endl;

				int command_sz = argv_vect2.size();
				char** argv = new char*[command_sz + 1]; 								//create argv for execvp
				for (int j = 0; j < argv_vect2.size(); ++j) {
					argv[j] = new char[sizeof(argv_vect2.at(j)) + 1];					//filling in each index of argv with
					strcpy(argv[j], argv_vect2.at(j));									//enough space for cstring commands
					//cout << "contents of argv: " << argv[j] << endl;
				}
				argv[argv_vect2.size()] = NULL;

				int pid2 = fork();
				if (pid2 == -1) {
					perror("Unable to fork for execvp.");
				}
				if (pid2 == 0) {
					if (-1 == execvp(argv[0], argv)) {
						perror("There was an error in execvp. ");
						exit(1);
					}
				}
				if (pid2 > 0) {
					if (-1 == wait(0)) {
						perror("Still waiting for child execvp process to end.");
					}
				}
				
				argv_tok = strtok_r(NULL, ";", &saveptr1);
				++i;
			//	cout << "end of line:"; 
			//	cout << argv_tok << "|test|" << endl;
				//	argv_vect.clear();
				argv_vect2.clear();
				/*for (int k = 0; k < argv_vect.size(); ++k ) {
					cout << "contents of parsed vector: " << argv_vect.at(k) << endl;
				}
				
				for (int l = 0; l < argv_vect2.size(); ++l) {
					cout << "contents of parsed vector 2: " << argv_vect2.at(l) << endl;	//verifying the parse has worked
				}*/
			}
		
			/*for (int i = 0; i < argv_vect.size(); ++i) {
				cout << "contents of parsed vector: " << argv_vect.at(i) << endl;	//verifying the parse has worked
			}*/

			/*for (int i = 0; i < argv_vect.size(); ++i) {
				char* argv_tok2 = strtok(argv_vect.at(i), " ");
				while (argv_tok2 != NULL) {
					argv_vect2.push_back(argv_tok2);
					argv_tok2 = strtok(NULL, " ");				
				}
			}
			*/
		


			/*int pid2 = fork();

			if (pid2 == -1) {
				perror("Second fork did not work");
			}

			if (pid2 == 0) {
				if (-1 == execvp(argv[0], argv)) {
					perror("There was an error in execvp. ");
					exit(1);
				}
			}

			if (pid2 > 0) {
				if (-1 == wait(0)) {
					perror("Waiting for 2nd child process to end.");
				}

			}*/
			exit(1);
		}
	}

	else if (pid > 0) {
		//parent
		if (-1 == wait(0)) {		//wait returns PID of child (-1 if theres an error)
			perror("You are still waiting for the child process to end.");	
		}
	}
}
