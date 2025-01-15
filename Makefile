# Flags:
MAKEFLAGS += --silent

# Builds:
build: bin bin/addr.o bin/main.o bin/server.o bin/client.o
	gcc -Werror -Wall bin/addr.o bin/main.o bin/server.o bin/client.o -o bin/ncc

test: bin bin/addr.o bin/server.o bin/test.o bin/client.o
	gcc -Werror -Wall bin/addr.o bin/test.o -o bin/ncc_test bin/server.o bin/client.o 
	tests/main.sh

# Cleaning:
clean:
	rm -f bin/*
	rm -rf bin/

# Dependencies:
bin/addr.o: src/addr.c
	gcc -Werror -Wall -ggdb src/addr.c -c -o bin/addr.o

bin/main.o: src/main.c
	gcc -Werror -Wall -ggdb src/main.c -c -o bin/main.o

bin/server.o: src/server.c
	gcc -Werror -Wall -ggdb src/server.c -c -o bin/server.o

bin/client.o: src/client.c
	gcc -Werror -Wall -ggdb src/client.c -c -o bin/client.o

bin/test.o:
	gcc -Werror -Wall -ggdb src/test.c -c -o bin/test.o

bin:
	mkdir bin/
