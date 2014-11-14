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

/*
#define TOKEN tokenizer<char_separator<char>>
#define seperator char_seperator<char>

const char semicolon[] = ";";
const char ands[] = "&&";
const char ors[] = "||";

*/

int main() {

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
		int i_flag = 0;
		int o_flag = 0;
		int oo_flag = 0;
		int pipe_flag = 0;

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
					and_flag = 1;
				}				
			}
			if (c_commands[cmtfind] == '|') {
				if (c_commands[cmtfind + 1] == '|') {
					or_flag = 1;
				}
				else {
					//piping
					pipe_flag = 1;
				}
			}
			if (c_commands[cmtfind] == '<') {
				//input redirection
				i_flag = 1;
			}
			if (c_commands[cmtfind] == '>') {
				if (c_commands[cmtfind + 1 ] == '>') {
					//>> don't delete whats in the file (the file you are redirecting the output into)
					oo_flag = 1;
				}
				else {
					//> delete whats in the file you are redirecting into
					o_flag = 1;
				}
			}
		}
	
		char delim_list[] = ";|&";
		char* argv_tok = strtok_r(c_commands, delim_list, &saveptr1);					//parse the c-string(user input)

		vector<char*> argv_vect;
		vector<char*> argv_vect2;
		while (argv_tok != NULL) {													//fill vectors with parsed user input
			argv_vect.clear();
			argv_vect.push_back(argv_tok);
			if (argv_vect.size() > 0) {
				for (unsigned argv_ctr = 0; argv_ctr < argv_vect.size(); ++argv_ctr) {
					cout << "contents of argv_vect1: " << argv_vect.at(argv_ctr) << endl;
				}
			}
			
			char* argv_tok2 = strtok_r(argv_vect.back(), " " , &saveptr2);
			while (argv_tok2 != NULL) {
				argv_vect2.push_back(argv_tok2);
				argv_tok2 = strtok_r(NULL, " ", &saveptr2);
			}
			for (unsigned argv2_ctr = 0; argv2_ctr < argv_vect2.size(); ++argv2_ctr) {
				cout << "contents of argv_vect2: " << argv_vect2.at(argv2_ctr) << endl;
			}
			
			int command_sz = argv_vect2.size();
			char** argv = new char*[command_sz + 1]; 								//create argv for execvp
			for (unsigned j = 0; j < argv_vect2.size(); ++j) {
				string exitor = "exit";
				string exitor2 = " exit";
				int exit_comp = strcmp(argv_vect2.at(j), exitor.c_str());
				int exit_comp2 = strcmp(argv_vect2.at(j), exitor2.c_str());
				if (exit_comp == 0 || exit_comp2 == 0) {
					exit(1);
				}
				
				argv[j] = new char[sizeof(argv_vect2.at(j)) + 1];					//filling in each index of argv with
				strcpy(argv[j], argv_vect2.at(j));									//enough space for cstring commands
			}

			for (unsigned argv3_ctr = 0; argv3_ctr < argv_vect2.size(); ++argv3_ctr) {
				cout << "contents of argv array: " << argv[argv3_ctr] << endl;
			}
		
			argv[argv_vect2.size()] = NULL;

			int pid2 = fork();
			int status = 0;
			if (pid2 == -1) {
				perror("Unable to fork for execvp.");
				exit(1);
			}
			else if (pid2 == 0) {
				cout << "execvp's target: " << argv[0] << endl;
				if (-1 == execvp(argv[0], argv)) {
					perror("There was an error in execvp. ");
					if (and_flag > 0) {
						exit(1); //exit the child process to continue with the parent
					}
					exit(1); //exit the child process
				}
				else { 
					if (or_flag > 0) {
						break;
					}
				}
						
			}
			else if (pid2 != 0) {
//				cout << "status of pid2 : " << pid2 << endl;
//				cout << "status before wait: " << WEXITSTATUS(status) << endl;
				if (-1 == wait(&status)) {
					perror("Still waiting for child execvp process to end.");
				}
				
//				bool child_died = WIFEXITED(status);
//				cout << "child died or not (1 = yes, 0 = no) : " << child_died << endl;
//				cout << "status after wait: " << WEXITSTATUS(status) << endl;

				if (WEXITSTATUS(status)!= 0 && and_flag > 0) { //if execvp failed and the and_flag is up, then exit
					break;	
				}
					
				if (WEXITSTATUS(status) == 0 && or_flag > 0) { //if execvp succeeded and the or_flag is up, then exit
					break;
				}
	
				else {
						argv_tok = strtok_r(NULL, delim_list, &saveptr1);
						argv_vect2.clear();
						cout << " ---------------------------------" << endl;
				}
//				}
			}

		}

		cout << "argv_tok is null so we exit here" << endl;
	}
}


