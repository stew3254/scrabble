#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "server.h"

#define ADDR "0.0.0.0"
#define PORT 1337
#define MAX_CLIENTS 8
#define BUFFER_SIZE 4096


int main(int argc, char *argv[]) {
  //Initialize client list
  Client clients[MAX_CLIENTS] = {};

  //Buffer
  char buffer[BUFFER_SIZE+1] = {};

  int sock, index;

  //Start server
  if ((sock = setup_server(ADDR, PORT, MAX_CLIENTS)) == -1)
    return -1;

  //Wait for people to connect
  while(true) {
    get_output(sock, clients, MAX_CLIENTS, &index, buffer);
    if (buffer != NULL) {
      printf("%s", buffer);
      memset(buffer, 0, BUFFER_SIZE);
    }
  }
  return 0;
}
