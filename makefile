CFLAGS = -Wall -g

SERVER = filed
CLIENTS = newKey fileGet fileDigest 

all: $(SERVER) $(CLIENTS)

.PHONY: clean
clean:
	/bin/rm -rf *.o filed newKey fileGet fileDigest

zip:
	zip project5.zip *.c README