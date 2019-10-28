#ifndef scrabble_h_
#define scrabble_h_

#include "server/server.h"

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

//Starts the scrabble server
int scrabble_server(const char *ip, const int port, const int max_clients);

//Connects to scrabble server
Client scrabble_connect(const char *ip, const int port, char **buffer, const int len);

#endif
