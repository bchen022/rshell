#flags
FLAGS = -W -Wall -Werror -ansi -pedantic

all: rshell 

rshell:
		([ ! -d bin ] && mkdir bin) || [ -d bin ]
		g++ $(FLAGS) src/main.cpp -o bin/rshell

clean:
		rm -rf bin
