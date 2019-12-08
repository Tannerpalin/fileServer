# fileServer
Project 5, CS270: Systems Programming with Dr.Raphael Finkel, a file server written in C.<br/> Authors: Tanner Palin and Will Shapiro.

## Server
Our actual file server is found within the `filed.c` file.

## API Client methods
We have four different files which will make different requests to our server found in `filed.c`
These three methods can be found in: `newKey.c`, `fileGet.c`, `fileDigest.c`, and `fileRun.c`

## Compilation
* Running `make` from the command line will compile the server as well the Client methods.
* Running `make clean` will clean up all resulting executables and object files.
* Running `make zip` will zip all of our `.c` files and `README.md` into a `project5.zip` for submission.

### `newKey` Usage
When performing a newKey request, the execution from the command line should like:
```bash
./newKey machineName port secretKey newSecretKey
```
* Where `machineName` is the IP Address of DNS Name of the machine that the server is running on.
* `port` is the port number that the server will be listening to on the machine.
* `secretKey` is the current secret key (a 32-bit unsigned integer) that the server is using to authenticate requests.
* `newSecretKey` is the new secret key (32-bit unsigned integer) that the server will use upon success of the request.


### `fileGet` Usage
When performing a fileGet request, the execution from the command line should like:
```bash
./fileGet machineName port secretKey fileName
```
* Where `machineName` is the IP Address of DNS Name of the machine that the server is running on.<br/>
* `port` is the port number that the server will be listening to on the machine.
* `secretKey` is the current secret key (a 32-bit unsigned integer) that the server is using to authenticate requests.<br/>
* `fileName` is the name of a file (100 character maximum) stored on the server.
Upon success of this request, the client will receive the first 100 bytes of the file stored on the server.

### `fileDigest` Usage
When performing a fileDigest request, the execution from the command line should like:
```bash
./fileDigest machineName port secretKey fileName
```
* Where `machineName` is the IP Address of DNS Name of the machine that the server is running on.<br/>
* `port` is the port number that the server will be listening to on the machine.
* `secretKey` is the current secret key (a 32-bit unsigned integer) that the server is using to authenticate requests.<br/>
* `fileName` is the name of a file (100 character maximum) stored on the server.
Upon success of this request, the client will receive a 100 byte cryptographic digest of the file stored on the server.


### `fileRun` Usage


## Examples
A working example of executables can be found in the `finkelServer` folder. Use `localhost` of `127.0.0.1` for the machine name in the command line of your client applications if the server and client are running on the same machine.
