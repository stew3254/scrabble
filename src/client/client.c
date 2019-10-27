#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "../scrabble.h"
#include "../net.h"

#define ADDR "127.0.0.1"
#define PORT 9000
#define BUFFER_LEN 4096
#define MAX_FDS 2
#define STDIN 0

int get_activity(int fds[], const int max_fds, char **buffer, int *len) {
  //File descriptor set
  fd_set rfds;

  //Clear the socket set
  FD_ZERO(&rfds);

  int max_fd;

  //Add child sockets to set
  for (int i = 0 ; i < max_fds; ++i) {
    //If valid socket descriptor then add to read list
    if (fds[i] > -1)
      FD_SET(fds[i], &rfds);

    //Highest file descriptor number, need it for the select function
    if (fds[i] > max_fd)
      max_fd = fds[i];
  }

    //Wait for an activity on one of the sockets, timeout is NULL
  int activity = select(max_fd + 1, &rfds, NULL, NULL, NULL);

  //Check if there is an error and it's not a signal interrupt
  if ((activity < 0) && (errno!=EINTR)) {
    fprintf(stderr, "select error");
    return -2;
  }

  for (int i = 0; i < max_fds; ++i) {
    if (FD_ISSET(fds[i], &rfds)) {
      if ((*len = read(fds[i], *buffer, BUFFER_LEN)) == 0) {
        //Close the socket and mark as 0 in list for reuse
        if (i == 1) {
          close(fds[i]);
          memset(fds + i, 0, sizeof(int));
          return -1;
          if (*buffer != NULL) {
            free(*buffer);
            *buffer = NULL;
          }
        }
      }
      else {
        //Someone sent a message and return the buffer
        return i;
      }
      return i;
    }
  }
}

int main(int argc, char *argv[]) {
  char *ip = ADDR;
  int port = PORT;
  if (argc > 2) {
    if (is_digit(argv[2]) && port > 0 && port < 65536) {
      port = atoi(argv[2]);
    }
    else {
      fprintf(stderr, "Must be a valid port");
      return -1;
    }
  }

  int len = 0;
  char *buffer = (char*) calloc(BUFFER_LEN+1,sizeof(char));
  Client c = scrabble_connect(ip, port, &buffer, BUFFER_LEN);
  if (c.sock == -1) {
    fprintf(stderr, "Failed to connect!\n");
    return -1;
  }
  else {
    printf("Connected!\n");
  }


  int fds[MAX_FDS] = {STDIN, c.sock};
  while (true) {
    int index = get_activity(fds, MAX_FDS, &buffer, &len);
    if (index == 0) {
      if (is_quit(buffer, BUFFER_LEN)) {
        quit(&c);
        close(c.sock);
        printf("Exiting");
        return 0;
      }
      write(c.sock, buffer, BUFFER_LEN);
      memset(buffer, 0, BUFFER_LEN);
    }
    if (index == 1) {
      if (buffer != NULL) {
        printf("%s", buffer);
        memset(buffer, 0, BUFFER_LEN);
      }
      else {
        buffer = (char*) calloc(BUFFER_LEN+1,sizeof(char));
      }
    }
  }

  return 0;
}
