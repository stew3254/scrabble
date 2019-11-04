server: src/server/*.c src/scrabble*.c src/net.c
	gcc -o bin/server src/server/*.c src/scrabble*.c src/net.c

client: src/server/server.c src/client/*.c src/scrabble*.c src/net.c
	gcc -o bin/client src/server/server.c src/client/*.c src/scrabble*.c src/net.c

debug-server: src/server/*.c src/scrabble*.c src/net.c
	gcc -g -o bin/server src/server/*.c src/scrabble*.c src/net.c

debug-client: src/server/server.c src/client/*.c src/scrabble*.c src/net.c
	gcc -g -o bin/client src/server/server.c src/client/*.c src/scrabble*.c src/net.c
