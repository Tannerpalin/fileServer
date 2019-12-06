# fileServer
Project 5, CS270: Systems Programming with Dr.Raphael Finkel, a file server written in C.

## Server
Our actual server is found within the `filed.c` file.

## API Client methods
We have three different files which will make different requests to our server found in `filed.c`
These three methods can be found in: `newKey.c`, `fileGet.c`, and `fileDigest.c`

## Compilation
Running `make` from the command line will compile the server as well the Client methods.
Running `make clean` will clean up all resulting executables and object files.
Running `make zip` will zip all of our `.c` files into a `project5.zip` for submission.

## Examples
A working example can be found in the `exampleServer` folder. Use `localhost` in the command line for your clients connection. The
`exampleServer` does not contain any useful code to help us out. While `echoServer` has some very useful code to use.
