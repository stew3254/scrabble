#ifndef scrabble_h_
#define scrabble_h_

#include "server/server.h"

//Given a client, constructs a proper hello and sends it to the client
void hello(Client *c);

//Checks if a given message is a proper hello
bool is_hello(const char *msg, const int len);

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
