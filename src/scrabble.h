#ifndef scrabble_h_
#define scrabble_h_

#include "server/server.h"

#define BUFFER_LEN 4096
#define BOARD_HEIGHT 15
#define BOARD_WIDTH 15

static struct command {
  const char *hello;
  const char *quit;
  const char *ok;
  const char *nok;
  const char *userset;
  const char *userchange;
  const char *userjoin;
  const char *ready;
  const char *starting;
  const char *score;
  const char *boardpush;
  const char *tiles;
  const char *turn;
  const char *place;
  const char *pass;
  const char *exchange;
  const char *winner;
} Command = {
  .hello = "HELLO",
  .quit = "QUIT",
  .ok = "OK",
  .nok = "NOK",
  .userset = "USERSET",
  .userchange = "USERCHANGE",
  .userjoin = "USERJOIN",
  .ready = "READY",
  .starting = "STARTING",
  .score = "SCORE",
  .boardpush = "BOARDPUSH",
  .tiles = "TILES",
  .turn = "TURN",
  .place = "PLACE",
  .pass = "PASS",
  .exchange = "EXCHANGE",
  .winner = "WINNER",
};

typedef struct tile {
  int modifier;
  int x;
  int y;
  char letter;
} Tile;

//Does a quick check to see if the command might be valid
bool check_command(const char* msg, const int len);

//Given a client, constructs a proper hello and sends it to the client
void hello(Client *c);

//Checks if a given message is a proper hello
bool is_hello(const char *msg, const int len);

//Sends an okay
void ok(const Client *c, char *msg, const int len);

//Checks if a given message is a proper okay
void is_ok(const char *msg, const int len);

//Sends not okay
void nok(const Client *c, char *msg, const int len);

//Checks if a given message is a proper not okay
void is_nok(const char *msg, const int len);

//Given a client, constructs a proper goodbye and closes connection to the client
void goodbye(Client *c);

//Quits and closes the connection
void quit(Client *c);

//Checks if something is quit
bool is_quit(const char *msg, const int len);

//Makes a tile into a string
void tile_to_str(const Tile *t, char *s);

//Makes a tile into a string
void str_to_tile(Tile *t, const char *s);

//Sends the board to everyone
void board_push(const Client *clients, const int max_clients, Tile **board);

//Check if something is a board
bool is_board(const char *msg, const int len);

//Read in the board
void read_board(Tile **board, const char *msg);

//Checks if is place
bool is_place(const char *msg, const int len);

//TODO fix this
//Places a tile
void place(Client *c, const Tile *tiles, const int len);

//Checks if someone is a winner
bool is_winner(const char *msg, const int len);

//TODO fix this
int get_winner(const Client *clients, const int max_socks);

//Pass userlist and the index of the winner
void winner(const Client *clients, const int max_socks, const int index);

//Starts the scrabble server
int scrabble_server(const char *ip, const int port, const int max_clients);

//Connects to scrabble server
Client scrabble_connect(const char *ip, const int port, char **buffer, const int len);

#endif
