#include <iostream>
#include <sys/types.h>		//fork, waitpid
#include <unistd.h>			//fork, execvp
#include <sys/wait.h>		//waitpid

using namespace std;

int main() {

	int pid = fork();
	if (pid == 0) {

		//child
		//executes ls (execvp)

	}

	else {

		//parent
		//wait(NULL)

	}

}
