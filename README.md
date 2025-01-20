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

This project is heavily inspired by Beej's Guide to Networking, and uses examples from it where noted: https://beej.us/guide/bgnet/
