
COMPILER = g++
FLAGS = -Wall -Werror -ansi -pedantic

all: src/main.cpp
	mkdir bin
	g++ $(FLAGS) src/main.cpp -o bin/rshell


rshell: src/main.cpp
	mkdir bin
	g++ $(FLAGS) src/main.cpp -o bin/rshell

clean:
	rm -rf bin
	
