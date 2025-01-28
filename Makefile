# Flags:
MAKEFLAGS += --silent

# Builds:
build: bin bin/addr.o bin/server.o bin/client.o lib target
	mv bin/libnetlib.a target/
	rm -f bin/*
	rm -rf bin/

test: bin bin/addr.o bin/server.o bin/test.o bin/client.o libtest
	gcc -Werror -Wall -L./bin -lnetlib -o bin/ncc_test 
	tests/main.sh

lib:
	ar rcs bin/libnetlib.a bin/addr.o bin/server.o bin/client.o

libtest:
	ar rcs bin/libnetlib.a bin/test.o bin/addr.o bin/server.o bin/client.o

# Cleaning:
clean:
	rm -f bin/*
	rm -rf bin/
	rm -rf target/
	rm test.file test.log random.data 

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

target:
	mkdir target/
