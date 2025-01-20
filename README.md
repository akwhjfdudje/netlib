# netlib

This is a minimal networking library written in C, using unix sockets.

Currently this project only has support for:
- TCP

WIP:
- Concurrency
- UDP support
- Documentation

To build (WIP), just run
```
make
```

To run tests
```
make clean
make test
```

To use the library, you can link the static library when compiling your program
```
gcc -o mynetprogram -L. -lnetlib
```

This project is heavily inspired by Beej's Guide to Networking, and uses examples from it where noted: https://beej.us/guide/bgnet/
