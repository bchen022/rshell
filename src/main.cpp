#include <iostream>
#include <sys/types.h>		//fork, waitpid, open
#include <unistd.h>			//fork, execv
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
#include <signal.h>

using namespace std;
using namespace boost;


#define TOKEN tokenizer<char_separator<char> >
#define SEPARATOR char_separator<char>


void sig_int(int sig_num) {
	sig_num++;
}
void sig_stop(int sig_num) {
	sig_num++;
	pid_t pid = getpid();
	kill(pid, SIGSTOP);
}


int main() {
	unsigned cd_break = 0;
	while(1) {

		signal(SIGINT, sig_int);
		signal(SIGTSTP, sig_stop);
/*		string userid = getlogin();
		if (NULL == getlogin()) {
			perror("Getlogin() could not retreive a username.");
		}
		char hostname[256];
		if (-1 == gethostname(hostname, sizeof(hostname))) {
			perror("Gethostname() could not retreive a hostname.");
		}
		cout << userid << "@" << hostname;
*/		
		char* the_cwd = new char[1024];
		if (getcwd(the_cwd, 1024) == NULL) {
			perror("getcwd prompt");
		}
		cout << the_cwd << "$ ";

		string commands;
		string semicolon = ";";
		string ands = "&&";
		string ors = "||";
		string inputs = "<";
		string outputs_change = ">";
		string outputs_keep = ">>";
		string pipes = "|";

		int first_rshell_flag = 0;
		int new_rshell_flag = 0;
		int and_flag = 0;
		int or_flag = 0;
		int i_flag = 0;
		int o_flag = 0;
		int oo_flag = 0;
		int num_of_pipes_left = 0;

		string tok_this;
		vector<string> allpaths;
		getline(cin, commands);														
		string env(getenv("PATH"));
/*		cout << "env: " << env << endl;
		cout << "--------------------------" << endl;
*/		string temppath = "";
		for (unsigned i = 0; i < env.length(); ++i) {
			if (env.at(i) == ':' || (i == env.length()-1)) {
				if (i == env.length() - 1) {
					temppath = temppath + env.at(i);
				}
				temppath = temppath.append("/");
				allpaths.push_back(temppath);
				temppath = "";
			}
			else {
				temppath = temppath + env.at(i);
			}
		}

/*		for (unsigned i = 0; i < allpaths.size(); ++i) {
			cout << allpaths.at(i) << endl;
		}

		cout << "----------------------------" << endl;
*/		char* c_commands = new char [commands.length() + 1];						//create a c-string for strtok's argument
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
					new_rshell_flag = 1;
					++num_of_pipes_left;
					tok_this = pipes;
				}
			}
			if (c_commands[cmtfind] == ';'){
				first_rshell_flag = 1;
				tok_this = semicolon;
			}
			if (c_commands[cmtfind] == '<') {
				new_rshell_flag = 1;
				tok_this = inputs;
			}
			if (c_commands[cmtfind] == '>') {
				if (c_commands[cmtfind + 1 ] == '>') {
					new_rshell_flag = 1;
					tok_this = outputs_keep;
				}
				else {
					new_rshell_flag = 1;
					tok_this = outputs_change;
				}
			}
		}
		if (first_rshell_flag == 0 && new_rshell_flag == 0) {
			first_rshell_flag = 1;
		}

		if (first_rshell_flag > 0) {
			SEPARATOR sep(tok_this.c_str());
			TOKEN tok(commands, sep);
			vector<string> argv_vect;
			TOKEN::iterator temp_it = tok.begin();
			for (TOKEN::iterator it = tok.begin(); it != tok.end(); ++it) {
				if ((*it) == "exit") {
//					cout << "1" << endl;
					exit(1);
				}
//				cout << "temp_it: " << *temp_it << endl;
				if ((*it).at(0) == 'c' && (*it).at(1) == 'd') {
//					cout << "it is cd" << endl;
					char* path_cd = new char[1024];	
					char* cwd = new char[1024];
					char* cwd2 = new char[1024];
					unsigned slashindex;
					if (getcwd(cwd, 1024) == NULL) {
						perror("getcwd");
					}
					
					if ((*it).c_str()[2] == '\0') {
//						cout << "a" << endl;
						cwd2 = getenv("HOME");
						delete [] cwd;
						path_cd = cwd2;
					}
					else if ((*it).at(3) == '.') {
						if((*it).c_str()[4] == '.') {
//							cout << "c" << endl;
							if ((*it).c_str()[5] == '\0') {
								for (unsigned i = 0; cwd[i] != '\0'; ++i) {
									if (cwd[i] == '/') {
										slashindex = i;
									}
								}
								for (unsigned i = 0; i < slashindex; ++i) {
									cwd2[i] = cwd[i];
								}
								delete [] cwd;
								path_cd = cwd2;
							}
							else {
								char* the_full_path2 = new char[1024];
								unsigned j = 0;
								for (unsigned i = 3; (*temp_it).c_str()[i] != '\0'; ++i, ++j) {
									the_full_path2[j] = (*temp_it).c_str()[i];
								}
								string the_full_path = the_full_path2;
								delete [] the_full_path2;
								//char* temp = new char[the_full_path.length() + 1];
								//memcpy(temp, the_full_path.c_str(), the_full_path.length());
								//strcpy(cwd2, cwd);
								strcpy(cwd2, the_full_path.c_str());
								delete [] cwd;
								path_cd = cwd2;
							}
						}
						else {
//							cout << "b" << endl;
							path_cd = cwd;
						}
					}
					
					else {
//						cout << "d" << endl;
						char* the_full_path2 = new char[1024];
						unsigned j = 0;
						for (unsigned i = 3; (*temp_it).c_str()[i] != '\0'; ++i, ++j) {
							the_full_path2[j] = (*temp_it).c_str()[i];
						}
						string the_full_path = the_full_path2;
						delete [] the_full_path2;
						//char* temp = new char[the_full_path.length() + 1];
						//memcpy(temp, the_full_path.c_str(), the_full_path.length());
						//strcpy(cwd2, cwd);
						strcpy(cwd2, the_full_path.c_str());
						delete [] cwd;
						path_cd = cwd2;
					}

//					cout << "path_cd: " << path_cd << endl;
					if (-1 == chdir(path_cd)) {
						perror("chdir");
					}
					temp_it = tok.begin();
					cd_break++;
					//break;
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
					TOKEN::iterator it2 = tok2.begin();
//					TOKEN::iterator temp_it2 = tok2.begin();
					if ((*it2) == "exit") {
//						cout << "2" << endl;
						exit(1);
					}
				
					if (cd_break > 0) {
						exit(1);
					}

					else {
						while (allpaths.size() > 0) {
							unsigned first_argv = 0;
							unsigned j = 0;
							for (it2 = tok2.begin(); it2 != tok2.end(); ++it2, ++j) {
								string the_arg = (*it2);
								if (first_argv == 0) {
									string the_path = allpaths.at(0);
									the_arg = the_path.append(the_arg);
									++first_argv;
								}	
								argv[j] = new char[the_arg.size() + 1];
								strcpy(argv[j], (the_arg.c_str()));
							}
				
							argv[j] = NULL;
/*							cout << "array: " << endl;
							for (unsigned i = 0; argv[i] != NULL; ++i) {
								cout << argv[i] << endl;
							}
*/	
							if (-1 == execv(argv[0], argv)) {
								if (allpaths.size() == 1) {
									perror("execv first shell");
									exit(1);
								}
							}
							allpaths.erase(allpaths.begin());
						}
					}
				}	
				else if (pid != 0) {
	//				cout << "status of pid : " << pid << endl;
	//				cout << "status before wait: " << WEXITSTATUS(status) << endl;
//					cout << "before the parent." << endl;
					if (-1 == wait(&status)) {
						perror("child old rshell");
						exit(1);
					}
					if (cd_break > 0) {
						cd_break = 0;	
					}

//					cout << "after the wait." << endl;
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
//			cout << "new" << endl;
			int savestdin;
			int savestdout;
			if (-1 == (savestdin = dup(0))) {
				perror("Could not create restoring point of stdin.");
			}
			if (-1 == (savestdout = dup(1))) {
				perror("Could not create restoring point of stdout.");
			}
			int redirection_flag = 0;
			tok_this = pipes;
			string tok_this2;
			SEPARATOR sep(tok_this.c_str());
			TOKEN tok(commands, sep);
			vector<string> argv_vect;
			vector<string> argv_vect2;
			char* argv[80];
			char* temp_argv[80];
			unsigned j = 0;
			for (TOKEN::iterator it = tok.begin(); it != tok.end(); ++it, ++j) {
				if ((*it) == "exit") {
					exit(1);
				}
				argv_vect.push_back(*it);

				const char* pipe_tok = argv_vect.back().c_str();
				for (unsigned flag_find = 0; flag_find < strlen(pipe_tok); ++flag_find) {
					if (pipe_tok[flag_find] == '<') {
						i_flag = 1;
						tok_this = inputs;
						redirection_flag = 1;
						break;
					}
					if (pipe_tok[flag_find] == '>') {
						if (pipe_tok[flag_find + 1] == '>') {
							oo_flag = 1;
							tok_this = outputs_keep;
							redirection_flag = 1;
							break;
						}
						else {
							o_flag = 1;
							tok_this = outputs_change;
							redirection_flag = 1;
							break;
						}
						break;
					}
				}
				if (redirection_flag == 0) {
					tok_this = " ";
				}
				
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
					TOKEN:: iterator it3 = tok3.begin();
					for (it3 = tok3.begin(); it3 != tok3.end(); ++it3, ++j) {
						argv[j] = new char[(*it3).size() + 1];
						strcpy(argv[j], (*it3).c_str());					
					}
				}
				if ((o_flag > 0 || oo_flag > 0) && i_flag == 0) {
					temp_argv[0] = new char[strlen(argv[j-1]) + 1];
					strcpy(temp_argv[0], argv[j-1]);
					argv[j-1] = NULL;
				}
				else if ((o_flag > 0 || oo_flag > 0) && i_flag > 0 && num_of_pipes_left == 0) {
					temp_argv[0] = new char[strlen(argv[j-1]) + 1];
					strcpy(temp_argv[0], argv[j-1]);
					argv[j-1] = NULL;
				}
				else {	
					argv[j] = NULL;
				}
				
				if (i_flag > 0) {
					if (-1 == close(0)) {
						perror("Could not close stdin.");
						exit(1);
					}
					int fd0 = open(argv[1], O_RDONLY, 00700);
					if (fd0 == -1) {
						perror("Could not open file with file descriptors.");
						exit(1);
					}
					if (-1 == dup2(fd0, 0)){					//stdin becomes the read end of the file
						perror("Error with setting stdin to be the file.");
						exit(1);
					}
				}
				if (o_flag > 0) {
					if (-1 == close(1)) {
						perror("Could not close stdout.");
						exit(1);
					}
					int fd1 = open(temp_argv[0], O_RDONLY | O_WRONLY | O_CREAT | O_TRUNC, 00700);
					if (fd1 == -1) {
						perror("Could not open file with file descriptors.");
						exit(1);
					}
					if (-1 == dup2(fd1, 1)) {
						perror("Error with setting stdout to be the file.");
						exit(1);
					}
				}
				if (oo_flag > 0) {
					if (-1 == close(1)) {
						perror("Could not close stdout2.");
						exit(1);
					}
					int fd1 = open(temp_argv[0], O_RDONLY | O_WRONLY | O_CREAT | O_APPEND, 00700);
					if (fd1 == -1) {
						perror("Could not open file with file descriptors.");
						exit(1);
					}
					if (-1 == dup2(fd1, 1)) {
						perror("Error with setting stdout2 to be the file.");
						exit(1);
					}
				}

				if (num_of_pipes_left == 0) {
					int status2 = 0;
					int pid_no_pipe = fork();
					if (-1 == pid_no_pipe) {
						perror("Could not fork.");
						exit(1);
					}
					else if (pid_no_pipe == 0) {
/*						cout << "array: " << endl;
						for (unsigned i = 0; argv[i] != NULL; ++i) {
							cout << argv[i] << endl;
						}
*/						string true_the_arg = argv[0];
						while (allpaths.size() > 0) {
							string the_path = allpaths.at(0);
							string the_arg = true_the_arg;
							the_arg = the_path.append(the_arg);
							argv[0] = new char[the_arg.size() + 1];
							strcpy(argv[0], the_arg.c_str());
//							cout << "argv[0] full path: " << argv[0] << endl;						
							if (-1 == execv(argv[0], argv)) {
								if (allpaths.size() == 1) {
									perror("Error with execv(no pipes)");
									exit(1);
								}
							}
							allpaths.erase(allpaths.begin());
						}
					}
					else {
						if (-1 == wait(&status2)) {
							perror("Could not wait.");
							exit(1);
						}
						if (WEXITSTATUS(status2) == 0) {
							if (-1 == dup2(savestdin, 0)) {
								perror("Could not restore regular std in.");
								exit(1);
							}
							if (-1 == dup2(savestdout, 1)) {
								perror("Could not restore regular std out.");
								exit(1);
							}
//							cout << "child exited normally." << endl;
							break;
						}
					}
				}
				else {
					int status3 = 0;
					int fd_i[2];							//array that holds the fd of read/write of the created ifile
					if (-1 == pipe(fd_i)) {					//now fd_i[0] for reading from the pipe
						perror("Could not pipe.");			//fd_i[1] for writing from the pipe
						exit(1);
					}
					int pid_pipe = fork();
					if (pid_pipe == -1) {
						perror("Error in fork for pipe.");
						exit(1);
					}
					else if (pid_pipe == 0) {
						//make stdout the write end of the pipe to set up a pipe between the parent and child process
						if (-1 == close(1)) {
							perror("Could not close stdout.");
							exit(1);
						}
						if (-1 == dup2(fd_i[1], 1)) { 		//stdout becomes the write end of the pipe*/
							perror("Error with making stdout to the write end of pipe.");
							exit(1);
						}
						if (-1 == close(fd_i[0])) {			//don't need the read end of pipe for now
							perror("Error with closing read of end of pipe.");
							exit(1);
						}
						string true_the_arg = argv[0];
						while (allpaths.size() > 0) {
							string the_path = allpaths.at(0);
							string the_arg = true_the_arg;
							the_arg = the_path.append(the_arg);
							argv[0] = new char[the_arg.size() + 1];
							strcpy(argv[0], the_arg.c_str());

							if (-1 == execv(argv[0], argv)) {
								if (allpaths.size() == 1) {	
									perror("Error with execv.");
									exit(1);
								}
							}
							allpaths.erase(allpaths.begin());
						}
					}
					else if (pid_pipe > 0) {
						if (-1 == wait(&status3)) {						//do we need status
							perror("Error with wait.");
							exit(1);
						}
						//recursive call here? no because you still need to connect the other end of fd_i

						//make stdin the read end of the pipe to set up a pipe between the parent and child process
						if (-1 == close(0)) {
							perror("Could not close stdin.");
							exit(1);
						}
						if (-1 == dup2(fd_i[0], 0)) {		//stdin becomes the read end of the pipe
							perror("Error with making stdin to the read end of pipe.");
							exit(1);
						}
						if (-1 == close(fd_i[1])) {			//dont need the write end of pipe for now
							perror("Error with closing the write end of pipe.");
							exit(1);
						}
						//first pipe is now established					
						
						num_of_pipes_left--;
						i_flag = 0;
						o_flag = 0;
						oo_flag = 0;
						if (num_of_pipes_left == 0) {
							if (-1 == dup2(savestdout,1)){
								perror("Could not restore regular stdout.");
								exit(1);
							}
						}
					}//parent branch of first fork
				}//if pipes are up
			}//first tokenizer
		}//new rshell branch		
	}// while(1)			
} //main	
				
				
				
				
				
				
				
			

			



//			www.cise.ufl.edu/class/cop4600sp12/dsslides/shell(6).pdf
//			stackoverflow.com/questions/3930339/implementing-pipes-in-a-c-shell-unix

	




