#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "scrabble.h"

#define BUFFER_LEN 4096
#define VERSION "1.0.1"
#define AUTHOR "Ryan Stewart"

//Given a client, constructs a proper hello and sends it to the client
void hello(Client *c) {
  char msg[BUFFER_LEN+1] = "HELLO ";
  strcat(msg, VERSION);
  strcat(msg, ",");
  //Fill in the OS specific info later
  //#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
  //  /* UNIX-style OS. ------------------------------------------- */
  //
  //#endif
  strcat(msg, "Ubuntu 19.04,C,");
  strcat(msg, AUTHOR);
  strcat(msg, "\r\n");
  write(c->sock, msg, BUFFER_LEN);
  c->hello = true;
}

//Checks to see if a message is a valid hello
bool is_hello(const char *msg, int len) {
  //21 bytes is the maximum needed for the base hello string
  char hello[21] = "HELLO ";
  char *substr, *hello_substr;
  strcat(hello, VERSION);
  strcat(hello, ",,\r\n");
  //Makes sure the length is longer than the string
  if (len >= strlen(hello)) {
    int n = 0, pos;
    hello_substr = strstr(hello, ",");
    pos = hello_substr - hello;
    for (int i = 0; i < pos+1; ++i) {
      if (msg[i] != hello[i]) {
        return false;
      }
    }
    ++pos;

    //Find commas
    while (n < 2 && pos < len) {
      if (msg[pos] == ',')
        ++n;
      ++pos;
    }

    //If less than 2 commas
    if (n < 2)
      return false;

    //Find next \r\n
    while (pos < len && msg[pos] != '\r' && msg[pos] != '\n')
      ++pos;

    //Close if not proper end of line
    if (msg[pos] != '\r' && msg[pos] != '\n')
      return false;

    return true;
  }
  else {
    return false;
  }
}

//Given a client, constructs a proper goodbye and closes connection to the client
void goodbye(Client *c) {
  char msg[10] = "GOODBYE\r\n";
  write(c->sock, msg, strlen(msg)); 
  close(c->sock);
  memset(c, 0, sizeof(Client));
  printf("Host Disconnected\n");
}


//Quits and closes the connection
void quit(Client *c) {
  char msg[7] = "QUIT\r\n";
  write(c->sock, msg, strlen(msg)); 
  close(c->sock);
  memset(c, 0, sizeof(Client));
}

//Checks to see if a message is a valid quit (probably works)
bool is_quit(const char *msg, const int len) {
  //5 bytes is the maximum needed for the base quit string
  char quit[7] = "QUIT\r\n";
  //Makes sure the length is same as string
  if (len == strlen(quit) || len == strlen(quit)-1) {
    for (int i = 0; i < len; ++i) {
      //Check if thing after quit is not right after the word
      if (i == 4 && (msg[i] == '\r' || msg[i] == '\n'))
        return true;
      else if (msg[i] != quit[i])
        return false;
    }
    return true;
  }
  else {
    return false;
  }
}

//Starts the scrabble server
int scrabble_server(const char *ip, const int port, const int max_clients) {
  //Initialize client list
  Client *clients = (Client*) malloc((max_clients)*sizeof(Client));

  //Buffer
  char *buffer = (char*) malloc((BUFFER_LEN+1)*sizeof(char));
  memset(buffer, 0, BUFFER_LEN+1);

  int sock, index, len;

  //Start server
  if ((sock = setup_server(ip, port, max_clients)) == -1)
    return -1;

  //Wait for people to connect
  while(true) {
    //Wait for output
    get_output(sock, clients, max_clients, &index, &buffer, &len);

    //Problem accepting connection
    if (index == -2) {
      fprintf(stderr, "Accept error");
    }
    //Client disconnected
    else if (index == -1) {
      printf("Host disconnected\n");
    }
    //Client connected
    else if (index > -1 && buffer == NULL) {
      hello(&clients[index]);
    }
    //Client sent message
    else if (buffer != NULL) {
      printf("%d %s", len, buffer);
      //Check the if hello is correct
      if (clients[index].hello && is_hello(buffer, len))
        clients[index].hello = false;
      else if (clients[index].hello && !is_hello(buffer, len))
        goodbye(&clients[index]);
      else if (is_quit(buffer, len))
        goodbye(&clients[index]);

      memset(buffer, 0, BUFFER_LEN);
    }

    //Allocate a new buffer when one doesn't exist
    if (buffer == NULL) {
      buffer = (char*) calloc(BUFFER_LEN+1,sizeof(char));
    }
  }
  return 0;
}

//Connects to scrabble server
Client scrabble_connect(const char *ip, const int port, char **buffer, const int len) {
  Client *c = (Client*) calloc(1, sizeof(Client));

  //Create socket
  c->sock = socket(AF_INET, SOCK_STREAM, 0);
  if (c->sock == -1) {
    memset(c, 0, sizeof(Client));
    c->sock = -1;
    return *c;
  }
  memset(&c->sa, 0, sizeof(struct sockaddr_in));

  //Assign port and ip
  c->sa.sin_family = AF_INET;
  c->sa.sin_addr.s_addr = inet_addr(ip);
  c->sa.sin_port = htons(port);

  //Connect
  if (connect(c->sock, (struct sockaddr*)&c->sa, sizeof(c->sa)) != 0) {
    memset(c, 0, sizeof(Client));
    c->sock = -1;
    return *c;
  }
  int n = read(c->sock, *buffer, len);
  if (is_hello(*buffer, len)) {
    hello(c);
    memset(*buffer, 0, len);
  }
  else {
    quit(c);
    close(c->sock);
    memset(c, 0, sizeof(Client));
    c->sock = -1;
    free(*buffer);
    buffer = NULL;
  }
  return *c;
}
