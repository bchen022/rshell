#flags
FLAGS = -W -Wall -Werror -ansi -pedantic

all: rshell

rshell: cp
		([ ! -d bin ] && mkdir bin) || [ -d bin ]
		g++ $(FLAGS) src/main.cpp -o bin/rshell

cp: src/cp.cpp
	([ ! -d bin ] && mkdir bin) || [ -d bin ]
	g++ $(FLAGS) src/cp.cpp -o bin/cp

clean:
		rm -rf bin
