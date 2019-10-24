#ifndef server_h_
#define server_h_

#include <arpa/inet.h>
#include <stdbool.h>

//Client structure
typedef struct client {
  //Sock address
  struct sockaddr_in sa;
  //Client sock
  int sock;
  //Client length
  int len;
  //Need client hello to connect
  bool hello;
  
} Client;

//Starts the server and returns a master sock
int setup_server(const char *ip, const int port, const int max_clients);

/*
 * Gets the output of the server whenever an event happens
 * sock is the server sock
 * clients is the array of clients passed to operate on
 * max_clients is the max number you can have
 * index is the index in the array of which client was operated on
 * if -1, host disconnected. If -2, accept error
 * If the value of clients[i].sock == 0 then the host disconnected, Otherwise something happened
 * buffer is the message buffer from the client.
 * Will be null if a client connected or disconnected instead
 */
void get_output(int sock, Client clients[], const int max_clients, int *index, char ** buffer, int *len);
#endif
