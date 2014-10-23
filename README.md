File List
----------
Makefile
LICENSE
README.md
./src
./tests

/src:
-----------
main.cpp

/test: 
-----------

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

Summary
----------------------------------------------------------------------
This project is a c++ implementation of a basic command shell, which 
we called "rshell". It displays the currently logged in user as well
as the name of the machine the user is logged into, followed by the 
command prompt '$'. The rshell then takes in commands from the user,
being able to execute the commands that a normal shell would
be able to execute. It is able to accept multiple commands at once.

However, there are bugs in this program:

1) The command prompt does not handle commands of pure connectors. Rather than an error message being printed, the program will just reprompt the user.

2) In the script, getlogin for the userid will not work properly.

3) If echo-ing with quotation marks, the quotation marks are not removed. If commanded to echo "Hello", "Hello" will be printed rather than Hello.

4) If echo-ing with a '#', the user input will not echo anything pass the '#'. If commanded to echo he#llo, only he will be printed.

5) git functions will not work.

6) cd will not work since it is a part of the bash.

7) After running rshell within itself multiple times, resources will become temporarily unavailable.

8) Commands with single connectors (|, &) will run both commands that are connected.

9) Arrow keys will not work within the shell.














