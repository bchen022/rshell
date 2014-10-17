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
			vector<const char*> argv_vect;
			cout << "$ ";
			getline(cin, commands);									//get user input
			char* c_commands = new char [commands.length() + 1];	//create a c-string for strtok's argument
			strcpy(c_commands, commands.c_str());					//fill in c-string with user input
			
			const char* argv_tok = strtok(c_commands,";");				//parse the c-string(user input)
			while (argv_tok != NULL) {
				argv_vect.push_back(argv_tok);				
				argv_tok = strtok(NULL, " ;");
			}
			
			/*for (int i = 0; i < argv_vect.size(); ++i) {
				cout << "contents of parsed vector: " << argv_vect.at(i) << endl;
			}*/

			int command_sz = argv_vect.size();
			char** argv = new char*[command_sz*sizeof(char*)]; 
			for (int i = 0; i < argv_vect.size(); ++i) {
				argv[i] = new char[sizeof(argv_vect.at(i)) + 1];
				strcpy(argv[i], argv_vect.at(i));
				//cout << "contents of argv: " << argv[i] << endl;
			
				if (i = argv_vect.size()) {
					argv[i] = NULL;			
				}
			}
			
			if (-1 == execvp(argv[0], argv)) {
				perror("There was an error in execvp. ");
			}

				
		}
		exit(1);
	}

	else if (pid > 0) {
		//parent
		if (-1 == wait(0)) {		//wait returns PID of child (-1 if theres an error)
			perror("You are still waiting for the child process to end.");	
		}
	}
}
