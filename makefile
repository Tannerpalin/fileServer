CFLAGS = -Wall -g
LDLIBS = -lpthread

SERVER = filed
CLIENTS = newKey fileGet fileDigest fileRun

all: $(SERVER) $(CLIENTS)

.PHONY: clean
clean:
	/bin/rm -rf *.o filed newKey fileGet fileDigest fileRun

zip:
	zip project5.zip *.c README