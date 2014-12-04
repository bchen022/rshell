How to run file
---------------
Depending on the Operating System, OS X vs Unix, program is compiled with the `#include <wait.h>` library.

OS X will not compile but will need the removal of the `#include <wait.h>` library call. 

For full feature, please add current path to the rshell directory to your $PATH. This will enable you to use the ls features made in my personal ls shell.

Once you have cloned the directory to your local machine, follow the directions below:
--------------------------------------------------------------------------------------
1. `cd` into `rshell` directory

2. Call `make`

3. `cd` into `bin`

4. Call `rshell`

5. Can do steps 3 & 4 by calling `bin/rshell`

RShell will now open up instead of BASH and you can execute almost all to nothing commands as included in the regular terminal.
======================================================================
Summary (rshell)
======================================================================
This project is a c++ implementation of a basic command shell, which 
we called "rshell". It displays the currently logged in user as well
as the name of the machine the user is logged into, followed by the 
command prompt '$'. The rshell then takes in commands from the user,
being able to execute the commands that a normal shell would
be able to execute. It is able to accept multiple commands at once as well
as being able to accept piping and io redirection. Rshell can also handle cd for the most part.

However, there are bugs in this program:

1) The command prompt does not handle commands of pure connectors. Rather than an error message being printed, the program will just reprompt the user.

2) In the script, getlogin for the userid will not work properly.

3) If echo-ing with quotation marks, the quotation marks are not removed. If commanded to echo "Hello", "Hello" will be printed rather than Hello.

4) If echo-ing with a '#', the user input will not echo anything pass the '#'. If commanded to echo he#llo, only he will be printed.

5) git functions will not work.

6) cd will not work since it is a part of the bash.

7) After running rshell within itself multiple times, resources will become temporarily unavailable.

8) Command with the single connector (&) will run both commands that are connected.

9) Arrow keys will not work within the shell.

10) The shell can take in multiple connectors in one command line, but only if the connectors are of one type.

11) The shell can either only take in either commands with connectors or commands with io redirection + piping, not both.

12) Cannot handle < and >/>> in the same line without pipes. (e.g. cat < testfile > newfile1). Goes into infinite loop.

13) If the command is an input redirection but the user does not provide a file, the whole shell will exit instead of reprompting the user.

14) Parts of the text in the file that receives the output redirection may randomly be placed into the directory.

15) Input redirection does not work (will be fixed on my own time). 

16) cd command will only work if its the only command in the command line (e.g. cannot work along with piping).

======================================================================
Summary (ls)
======================================================================
This project is a c++ partial implementation of the bash command "ls". After running the program, the user shall enter the command ls, followed by either the -a, -l, -R flags, a combination of any of the flags, paths, files, or any combinations of these parameters. The ls command shall display the files and directories available, depending on what parameters are passed in, including their information such as permissions and modification times which also depend on what flags are passed in with the ls command.

To run, type in the command line: bin/ls followed by ls, and a combination of the available parameters listed above.

However, this program has bugs:

1) The program assumes that the terminal has a width of 80 characters and adjusts its output (in columns) based on this width.

2) When passed in multiple paths, the program will display the paths in a backwards order.

3) If the user tries to pass in a directory name in the command line (e.g. bin/ls ls test_directory), the program will treat it like a file rather than go in it as a directory. (But the user can pass in a path (e.g. class/classes/user).

4) In directories with a huge amount of files, the files are displayed alphabetically in a horizontal rather than a vertical manner.

5) Outputting with the -l flag does not output perfectly aligned columns.

6) With the -R flag, some display of paths will have an extra '/'.
















