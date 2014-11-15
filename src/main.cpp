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
#include <cstdlib>

using namespace std;
using namespace boost;


#define TOKEN tokenizer<char_separator<char> >
#define SEPARATOR char_separator<char>


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
		string semicolon = ";";
		string ands = "&&";
		string ors = "||";

		int and_flag = 0;
		int or_flag = 0;
/*		int i_flag = 0;
		int o_flag = 0;
		int oo_flag = 0;
		int pipe_flag = 0;
*/
		string tok_this;

		getline(cin, commands);														//get user input
//		char* saveptr1;
//		char* saveptr2;
		char* c_commands = new char [commands.length() + 1];						//create a c-string for strtok's argument
		strcpy(c_commands, commands.c_str());										//fill in the c-string with user input
		for (unsigned cmtfind = 0; cmtfind < strlen(c_commands); ++cmtfind) {
			if (c_commands[cmtfind] == '#') {
				c_commands[cmtfind] = '\0';
				commands = commands.substr(0, cmtfind);

			}
			if (c_commands[cmtfind] == '&') {
				if (c_commands[cmtfind + 1] == '&') {
					and_flag = 1;
					tok_this = ands;
				}				
			}
			if (c_commands[cmtfind] == '|') {
				if (c_commands[cmtfind + 1] == '|') {
					or_flag = 1;
					tok_this = ors;
				}
//				else {
					//piping
//					pipe_flag = 1;
//				}
			}
			if (c_commands[cmtfind] == ';'){
				tok_this = semicolon;
			}
/*			if (c_commands[cmtfind] == '<') {
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
*/
		}
	
/*		char delim_list[] = ";|&";
		char* argv_tok = strtok_r(c_commands, delim_list, &saveptr1);					//parse the c-string(user input)
*/	
		SEPARATOR sep(tok_this.c_str());
		TOKEN tok(commands, sep);
		vector<string> argv_vect;
		for (TOKEN::iterator it = tok.begin(); it != tok.end(); ++it) {
			if ((*it) == "exit") {
				exit(1);
			}
			int pid = fork();
			int status = 0;
			if (-1 == pid) {
				perror("Could not fork the process.");
				exit(1);
			}
			
			else if (pid == 0) {
				argv_vect.clear();
				argv_vect.push_back(*it);
		/*		if (argv_vect.size() > 0) {
					for (unsigned argv_ctr = 0; argv_ctr < argv_vect.size(); ++argv_ctr) {
						cout << "contents of argv_vect1: " << argv_vect.at(argv_ctr) << endl;
					}
				}
		*/	
				char* argv[80];
				SEPARATOR sep2(" ");
				TOKEN tok2(argv_vect.back(), sep2);
				unsigned j = 0;
				TOKEN::iterator it2 = tok2.begin();
				if ((*it2) == "exit") {
						exit(1);
				}			
				for (it2 = tok2.begin(); it2 != tok2.end(); ++it2, ++j) {
					argv[j] = new char[(*it2).size() + 1];
					strcpy(argv[j], (*it2).c_str());
				}
	
				argv[j] = NULL;

				if (-1 == execvp(argv[0], argv)) {
					perror("There was an error in execvp");
					exit(1);
				}
			}

			else if (pid != 0) {
//				cout << "status of pid : " << pid << endl;
//				cout << "status before wait: " << WEXITSTATUS(status) << endl;
				if (-1 == wait(&status)) {
					perror("Still waiting for child execvp process to end.");
					exit(1);
				}
				if (WEXITSTATUS(status) != 0 && and_flag > 0) {
					break;
				}
				else if (WEXITSTATUS(status) == 0 && or_flag > 0) {
					break;
				}

			}
	
		}
	}
}

