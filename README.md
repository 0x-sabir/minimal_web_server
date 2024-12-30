# minimal_web_server

This project is a basic multithreaded web server implemented in C. It is designed to handle multiple incoming client requests concurrently using a thread pool.

The server listens on a specified port (default: 5001) and responds to HTTP requests with a simple **"Response Generated from SERVER!"** message. This project demonstrates the fundamentals of network programming, multithreading, and synchronization in C.

## Features

**concurrency**: Handles multiple client connections simultaneously using pthreads.

**Thread Pool**: Efficiently manages threads for handling client requests.

**Basic HTTP Response**: Sends a simple HTTP 200 response to clients.

**Error Handling**: Includes basic error handling for socket operations and thread creation.

## Prerequisites

GCC or any compatible C compiler

Basic knowledge of C programming

POSIX-compliant system (Linux, macOS, etc.)

## How to Build

Clone the repository or copy the source code.

Compile the source code using the following command:
```bash
gcc server.c
```

How to Run

Run the compiled binary:
 ```bash
./a.out
```

The server will start and listen on port 5001 by default.

Use a web browser, curl, or any HTTP client to send requests to the server:

curl http://localhost:5001

