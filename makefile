server: src/server/*.c src/scrabble*.c
	gcc -o bin/server src/server/*.c src/scrabble*.c

client: src/client/*.c src/scrabble*.c
	gcc -o bin/client src/client/*.c src/scrabble*.c
