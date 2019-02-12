# Simple HTTP 1.0 Server
Project 1 for Computer System (COMP30023).
A simple static file server with minimum support of HTTP 1.0 written in C.

### Prerequisite
    sys/sendfile.h 

### Installation

    git clone git@github.com:CaviarChen/comp30023-2018-project-1.git
    cd comp30023-2018-project-1/
    make

### Usage
     ./server [port number] [path to web root]

**Example**  

    ./server 8080 ./test/www/

### Supports
    1. Concurrent file transfer through the use of multithreading
    2. Large file transfer without crashing unexpectedly 
