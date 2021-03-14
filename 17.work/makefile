all:
	gcc src/2.server.c -I include/ src/common.c -o bin/server
	gcc src/1.client.c -I include/ src/common.c -o bin/client
PHONY:
clean:
	rm bin/server bin/client
