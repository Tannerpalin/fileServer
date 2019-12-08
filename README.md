# fileServer
Project 5, CS270: Systems Programming with Dr.Raphael Finkel, a file server written in C.<br/> Authors: Tanner Palin and Will Shapiro.

## Server
Our actual file server is found within the `filed.c` file.

## API Client methods
We have three different files which will make different requests to our server found in `filed.c`
These three methods can be found in: `newKey.c`, `fileGet.c`, and `fileDigest.c`

## Compilation
Running `make` from the command line will compile the server as well the Client methods.
Running `make clean` will clean up all resulting executables and object files.
Running `make zip` will zip all of our `.c` files and `README.md` into a `project5.zip` for submission.

## Examples
A working example of executables can be found in the `finkelServer` folder. Use `localhost` of `127.0.0.1` for the machine name in the command line of your client applications if the server and client are running on the same machine.
