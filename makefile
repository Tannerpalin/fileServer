CFLAGS = -Wall -g

SERVER = filed
CLIENTS = newKey fileGet fileDigest fileRun 

all: $(SERVER) $(CLIENTS)

.PHONY: clean
clean:
	/bin/rm -rf *.o filed newKey fileGet fileDigest fileRun project5.tar

zip:
	tar -cf project5.tar *.c README.md makefile