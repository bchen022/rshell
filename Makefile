#flags
FLAGS = -W -Wall -Werror -ansi -pedantic

all: rshell cp ls

rshell: src/main.cpp
		([ ! -d bin ] && mkdir bin) || [ -d bin ]
		g++ $(FLAGS) src/main.cpp -o bin/rshell

cp: src/cp.cpp
		([ ! -d bin] && mkdir bin) || [ -d bin ]
		g++ $(FLAGS) src/cp.cpp -o bin/cp

ls: src/ls.cpp
		([ ! -d bin] && mkdir bin) || [ -d bin ]
		g++ $(FLAGS) src/ls.cpp -o bin/rshell


clean:
		rm bin/*.o bin/rshell.out
