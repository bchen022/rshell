#include <iostream>
#include <sys/types.h>		//fork, waitpid, open
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
#include <fcntl.h> 			//dup, open
#include <sys/stat.h>		//open

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
		string inputs = "<";
		string outputs_change = ">";
		string outputs_keep = ">>";
		string pipes = "|";

		int first_rshell_flag = 0;
		int and_flag = 0;
		int or_flag = 0;
		int i_flag = 0;
		int o_flag = 0;
		int oo_flag = 0;
		int pipe_flag = 0;

		string tok_this;

		getline(cin, commands);														
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
					first_rshell_flag = 1;
					tok_this = ands;
				}				
			}
			if (c_commands[cmtfind] == '|') {
				if (c_commands[cmtfind + 1] == '|') {
					or_flag = 1;
					first_rshell_flag = 1;
					tok_this = ors;
				}
				else {
					//piping
					pipe_flag = 1;
					tok_this = pipes;
				}
			}
			if (c_commands[cmtfind] == ';'){
				first_rshell_flag = 1;
				tok_this = semicolon;
			}
			if (c_commands[cmtfind] == '<') {
				//input redirection
				i_flag = 1;
				tok_this = inputs;
			}
/*			if (c_commands[cmtfind] == '>') {
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
		
		if (first_rshell_flag > 0) {	
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
	
		else {
			//parse by |, then by space? 
			//one step at a time nigga
			tok_this = pipes;
			string tok_this2;
			SEPARATOR sep(tok_this.c_str());
			TOKEN tok(commands, sep);
			vector<string> argv_vect;
			vector<string> argv_vect2;
			char* argv[80];
			unsigned j = 0;
			for (TOKEN::iterator it = tok.begin(); it != tok.end(); ++it, ++j) {
				if ((*it) == "exit") {
					exit(1);
				}
				argv_vect.push_back(*it);
				char* pipe_tok = argv_vect.back().c_str();
				for (unsigned flag_find = 0; flag_find < strlen(pipe_tok); ++flag_find) {
					if (pipe_tok[flag_find] == '<') {
						i_flag = 1;
						tok_this = inputs;
					}
					if (pipe_tok[flag_find] == '>') {
						if (pipe_tok[flag_find + 1] == '>') {
							oo_flag = 1;
							tok_this = outputs_keep;
						}
						else {
							o_flag = 1;
							tok_this = outputs_change;
						}
					}
				}
		
				if (i_flag > 0 || o_flag > 0 || oo_flag > 0) {
					SEPARATOR sep2(tok_this.c_str());
					TOKEN tok2(argv_vect.back(), sep2);
					unsigned j = 0;
					TOKEN::iterator it2 = tok2.begin();
					if ((*it2) == "exit") {
						exit(1);
					}			
					for (it2 = tok2.begin(); it2 != tok2.end(); ++it2) {
						argv_vect2.push_back(*it2);
						SEPARATOR sep3(" ");
						TOKEN tok3(argv_vect2.back(), sep3);
						for (TOKEN:: iterator it3 = tok3.begin(); it3 != tok3.end(); ++it3, ++j) {
							argv[j] = new char[(*it3).size() + 1];
							strcpy(argv[j], (*it3).c_str());					
						}
					}
					argv[j] = NULL;
					
					//now that argv is set, set up redirections for execvp
					if (i_flag > 0) {

					}

					if (-1 == execvp(argv[0], argv)) {
						perror("There was an error in execvp");
						exit(1);
					}			
				}
			}
			
			www.cise.ufl.edu/class/cop4600sp12/dsslides/shell(6).pdf
			stackoverflow.com/questions/3930339/implementing-pipes-in-a-c-shell-unix

	

































			int fd_i[2];							//array that holds the fd of read/write of the created ifile
			if (-1 == pipe(fd_i)) {					//now fd_i[0] for reading from the pipe
				perror("Could not pipe.");			//fd_i[1] for writing from the pipe
			}
			int pid_pipe = fork();
			if (pid_pipe == -1) {
				perror("Error in fork for pipe.");
				exit(1);
			}
			else if (pid_pipe == 0) {
				//open on file/close so you can set stdin to the file
				//setting stdin to the file might be required regardless of '<' in the command line
				if (i_flag > 0) {
					if (-1 == close(0)) {
						perror("Could not close stdin.");
					}
					int fd0 = open(filename.c_str(), O_RDONLY | O_CREAT, 00700);	//fd0 is the file 
					//set stdin to the file
					if (-1 == dup2(fd0, 0)){					//stdin becomes the read end of the file
						perror("Error with setting stdin to be the read end of the file.");
					}
				}

				//-------------------------------------------------------------------------------------
				//make stdout the write end of the pipe to set up a pipe between the parent and child process
				if (-1 == close(1)) {
					perror("Could not close stdout.");		
				}
				if (-1 == dup2(fd_i[1], 1)) { 		//stdout becomes the write end of the pipe*/
					perror("Error with making stdout to the write end of pipe.");
				}
/*				if (-1 == close(fd_i[0])) {			//don't need the read end of pipe for now
					perror("Error with closing read of end of pipe.");
				}
*/
				if (-1 == execvp(argv[0], argv)) {
					perror("Error with execvp.");
				}
				//exit(1); //not sure if we need this
			}

			else (pid_pipe > 0) {
/*				//mike said to never change file descriptors in the parent process
 				int true_stdin;
				if (-1 == (true_stdin = dup(0))) { 			//needed to restore for later on
					perror("Could not save the actual stdin.");
				}
*/				
				if (-1 == wait(0)) {						//do we need status
					perror("Error with wait.");
				}
				
				int fd_i2[2];
				if (-1 == pipe(fd_i2)) {
					perror("Could not pipe2.");
				}
				
				int pid_pipe2 = fork();
				if (-1 == pid_pipe2) {
					perror("Error in fork for pip2.");
					exit(1);
				}
				else if (pip_pipe2 == 0) {
					//set stdout to the file, not sure if necessary just like when setting the file as stdin
					if (o_flag > 0) {
						if (-1 == close(1)) {
							perror("Could not close stdout.");
						}				
						int fd1 = open(filename.c_str(), O_WRONLY | O_CREAT, 00700);
						//set stdout to the file
						if (-1 == dup2(fd1, 1)){	//makes newfd into the oldfd; makes stdout to fd1
							perror("Error with setting stdout to be the write end of file.");
						}
					}
					if (oo_flag > 0) {
						int fd1_1 = open(filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 00700);
						if (-1 == close(1)) {
							perror("Could not close stdout for >>.");
						}
						//set stdout to the file
						if (-1 == dup2(fd1, 1)){
							perror("Error with assigning the write end of file to stdout.");
						}
					}

					//----------------------------------------------------------------------	
					//make stdin the read end of the pipe to set up a pipe between the parent and child process
					if (-1 == close(0)) {
						perror("Could not close stdin.");
					}
					if (-1 == dup2(fd_i[0], 0)) {		//stdin becomes the read end of the pipe
						perror("Error with making stdin to the read end of pipe.");
					}
/*					if (-1 == close(fd_i[1])) {			//dont need the write end of pipe for now
						perror("Error with closing the write end of pipe.");
					}
*/
					//--------------------------------------------------------------------------	
					//make stdout the the write end of the second pipe
					if (-1 == dup2(fd_i2[1], 1)) {
						perror("Error withing making stdout into the write end of the second pipe.");
					}
				}

				else if (pid_pipe2 > 0) {
					if (-1 == wait(0)) {
						perror("Error with wait2.");
					}
				}
			}
		}
	}
}























































