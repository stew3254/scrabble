#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

#define BUFFER_LEN 4096

//Starts the server and returns a master sock
int setup_server(const char *ip, const int port, const int max_clients) {
  //Master sock
  int sock;

  //Create master socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    fprintf(stderr, "Failed to open socket");
    return -1;
  }

  //Sock address
  struct sockaddr_in sa = {};

  //Set server address to use IPv4 and bind to ADDR and PORT
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = inet_addr(ip);

  //Bind socket
  if ((bind(sock, (struct sockaddr*)&sa, sizeof(sa))) != 0) {
    fprintf(stderr, "socket bind failed\n");
    return -1;
  } 

  //Listen for max number of users
  if ((listen(sock, max_clients)) != 0) {
    fprintf(stderr, "Listen failed\n");
    return -1;
  }
  printf("Server listening\n");
  return sock;
}

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
void get_output(int sock, Client clients[], const int max_clients, int *index, char **buffer, int *len) {
  //File descriptor set
  fd_set rfds;

  //Max sock
  int max_sd;

  //Clear the socket set
  FD_ZERO(&rfds);

  //Add master socket to set
  FD_SET(sock, &rfds);
  max_sd = sock;

  //Add child sockets to set
  for (int i = 0 ; i < max_clients; ++i) {
    //If valid socket descriptor then add to read list
    if (clients[i].sock > 0)
      FD_SET(clients[i].sock, &rfds);

    //Highest file descriptor number, need it for the select function
    if (clients[i].sock > max_sd)
      max_sd = clients[i].sock;
  }

  //Wait for an activity on one of the sockets, timeout is NULL
  int activity = select(max_sd + 1, &rfds, NULL, NULL, NULL);

  //Check if there is an error and it's not a signal interrupt
  if ((activity < 0) && (errno!=EINTR)) {
    printf("select error");
  }

  //If something happened on the master socket, then its an incoming connection
  if (FD_ISSET(sock, &rfds)) {
    Client c = {};
    if ((c.sock = accept(sock, (struct sockaddr*)&c.sa, (socklen_t*)&c.len)) < 0) {
      perror("accept");
      *index = -2;
      if (*buffer != NULL) {
        free(*buffer);
        *buffer = NULL;
      }
    }

    //Inform user of socket number - used in send and receive commands
    //printf("New connection, socket fd is %d, ip is : %s, port : %d\n", c.sock, inet_ntoa(c.sa.sin_addr), ntohs(c.sa.sin_port));

    //Add new socket to array of sockets
    for (int i = 0; i < max_clients; ++i) {
      //If position is empty
      if (clients[i].sock == 0) {
        memcpy(&clients[i], &c, sizeof(c));
        *index = i;
        if (*buffer != NULL) {
          free(*buffer);
          *buffer = NULL;
        }
        break;
      }
    }
  }

  //Else its some IO operation on some other socket
  for (int i = 0; i < max_clients; ++i) {
    if (FD_ISSET(clients[i].sock, &rfds)) {
      //Check if it was for closing, and also read the incoming message
      //If connection closed
      if ((*len = read(clients[i].sock, *buffer, BUFFER_LEN)) == 0) {
        //Keep this in case needed
        //getpeername(clients[i].sock, (struct sockaddr*)&clients[i].sa, (socklen_t*)&clients[i].len);
        //printf("Host disconnected, ip %s, port %d\n", inet_ntoa(clients[i].sa.sin_addr), ntohs(clients[i].sa.sin_port));

        //Close the socket and mark as 0 in list for reuse
        close(clients[i].sock);
        memset(clients + i, 0, sizeof(Client));
        *index = -1;
        if (*buffer != NULL) {
          free(*buffer);
          *buffer = NULL;
        }
      }
      else {
        //Someone sent a message and return the buffer
        *index = i;
      }
    }
  }
}
