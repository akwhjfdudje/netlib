build: bin/ bin/addr.o bin/main.o #bin/server.o bin/client.o
	gcc -Werror -Wall bin/addr.o bin/main.o bin/server.o bin/client.o -o bin/ncc

test: bin/ bin/test.o bin/addr.o #bin/server.o bin/client.o
	gcc -Werror -Wall bin/addr.o bin/test.o -o bin/ncc_test #bin/server.o bin/client.o 

clean:
	rm -f bin/*
	rm -rf bin/

bin/addr.o: src/addr.c
	gcc -Werror -Wall src/addr.c -c -o bin/addr.o

bin/main.o: src/main.c
	gcc -Werror -Wall src/main.c -c -o bin/main.o

bin/server.o: src/server.c
	gcc -Werror -Wall src/server.c -c -o bin/server.o

bin/client.o: src/client.c
	gcc -Werror -Wall src/client.c -c -o bin/client.o

bin/test.o:
	gcc -Werror -Wall src/main.c -DTEST -c -o bin/test.o

bin/:
	mkdir bin/