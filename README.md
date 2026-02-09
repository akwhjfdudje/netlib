# netlib

A simple, robust C networking library for TCP and UDP communication.

## Quick Start

To build the static library (`libnet.a`) and run the test suite:

```bash
make
./test
```

## Features

- **Robust TCP**: Guaranteed data transfer and automatic framing.
- **Easy UDP**: Simple messaging and broadcast support.
- **Reliable**: Connection retries and socket timeouts.
- **Developer Friendly**: Structured logging and clean API.

## Documentation

- [**API Reference**](DOCUMENTATION.md): Detailed function descriptions.
- [**Usage Guide**](USAGE.md): Practical code examples.

## Integration

Include the headers from `include/` and link against `libnet.a`:

```bash
gcc main.c -Iinclude -L. -lnet -o my_app
```
