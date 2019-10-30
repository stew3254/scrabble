#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "scrabble.h"

#define BUFFER_LEN 4096
#define VERSION "1.0.1"
#define AUTHOR "Ryan Stewart"
#define BOARD_HEIGHT 15
#define BOARD_WIDTH 15

//Does a quick check to see if the command might be valid
bool check_command(const char* msg, const int len) {
  if (len >= strlen(Command.hello) && strncmp(Command.hello, msg, strlen(Command.hello)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.quit) && strncmp(Command.quit, msg, strlen(Command.quit)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.ok) && strncmp(Command.ok, msg, strlen(Command.ok)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.nok) && strncmp(Command.nok, msg, strlen(Command.nok)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.userset) && strncmp(Command.userset, msg, strlen(Command.userset)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.userchange) && strncmp(Command.userchange, msg, strlen(Command.userchange)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.userjoin) && strncmp(Command.userjoin, msg, strlen(Command.userjoin)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.ready) && strncmp(Command.ready, msg, strlen(Command.ready)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.starting) && strncmp(Command.starting, msg, strlen(Command.starting)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.score) && strncmp(Command.score, msg, strlen(Command.score)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.boardpush) && strncmp(Command.boardpush, msg, strlen(Command.boardpush)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.tiles) && strncmp(Command.tiles, msg, strlen(Command.tiles)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.turn) && strncmp(Command.turn, msg, strlen(Command.turn)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.place) && strncmp(Command.place, msg, strlen(Command.place)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.pass) && strncmp(Command.pass, msg, strlen(Command.pass)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.exchange) && strncmp(Command.exchange, msg, strlen(Command.exchange)) == 0) {
    return true;
  }
  else if (len >= strlen(Command.winner) && strncmp(Command.winner, msg, strlen(Command.winner)) == 0) {
    return true;
  }
  return false;
}

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

//TODO could redo this
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
    for (; pos < len && msg[pos] != '\r'; ++pos);

    //Close if not proper end of line
    if (pos >= len && msg[pos] != '\n')
      return false;
    return true;
  }
  else {
    return false;
  }
}

//Sends an okay
void ok(const Client *c, char *msg, const int len) {
  int new_len = len+5;
  char *okay = (char*) calloc(new_len, sizeof(char));
  strcpy(okay,"OK ");
  //Copy message over
  memcpy(okay+3, msg, len);
  memcpy(okay+new_len-2, "\r\n", sizeof(char)*2);
  write(c->sock, okay, sizeof(char)*new_len); 
  free(okay);
}

//Checks if a given message is a proper okay
void is_ok(const char *msg, const int len);

//Sends not okay
void nok(const Client *c, char *msg, const int len) {
  int new_len = len+6;
  char *nokay = (char*) calloc(new_len, sizeof(char));
  strcpy(nokay,"NOK ");
  //Copy message over
  memcpy(nokay+4, msg, len);
  memcpy(nokay+new_len-2, "\r\n", 2);
  write(c->sock, nokay, new_len); 
  free(nokay);
}

//Checks if a given message is a proper not okay
void is_nok(const char *msg, const int len);

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

//Makes a tile into a string
void tile_to_str(const Tile *t, char *s) {
  memcpy(s, "(", 1);
  strcat(s,&t->letter);
  strcat(s, ",");
  char n = t->modifier + '0';
  strcat(s, &n);
  strcat(s, ")");
}

bool is_tile(const char *s) {
  char tile[6] = "(A,0)";
  for (int i = 0; i < 6; ++i) {
    if (i == 1 && (s[i] != '0' || s[i] - tile[i] < 0 || s[i] - tile[i] > 25))
      return false;
    else if (i == 3 && (s[i] < 0 || s[i] > 4))
      return false;
    else if (s[i] != tile[i])
      return false;
  }
  return true;
}

//Makes a tile into a string
void str_to_tile(Tile *t, const char *s) {
  t->letter = s[1];
  t->modifier = (int) s[3];
}

//Sends the board to everyone
void board_push(const Client *clients, const int max_clients, Tile **board) {
  char *str_board = (char*) calloc(BUFFER_LEN+1, sizeof(char));
  char *tile = (char*) calloc(6, sizeof(char));
  memcpy(str_board, "BOARDPUSH\r\n", 11);
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_HEIGHT; ++j) {
      tile_to_str(&board[i][j], tile);
      strcat(str_board, tile);
      memset(tile, 0, 6);
    }
    strcat(str_board, "\r\n");
  }
  for (int i = 0; i < max_clients; ++i) {
    if (clients[i].sock != 0) {
      write(clients[i].sock, str_board, BUFFER_LEN);
    }
  }
  free(str_board);
  free(tile);
}

//Read in the board
void read_board(const char *msg, const int len) {

}

//Checks if someone is a winner
bool is_winner(const char *msg, const int len) {
  int n = strlen(Command.winner);
  //Make sure message starts with "WINNER "
  if (len > n && strncmp(msg, Command.winner, n) == 0 && msg[n] == ' ') {
    ++n;
    //Look through string until it ends or hits a space
    for (; n <= len && msg[n] >= '0' && msg[n] <= '9'; ++n);

    //Check if it's too long or not a space
    if (n > len)
      return false;
    else if (msg[n] != ' ')
      return false;
    ++n;

    //Look for the '\r'
    int name_len = 0;
    for (; n < len && name_len <= NAME_LEN && msg[n] != '\r'; ++n,++name_len);
    if (name_len > NAME_LEN && n != len && msg[n] != '\n')
      return false;
    return true;
  }
  return false;
}

//TODO fix this
int get_winner(const Client *clients, const int max_clients) {
  int max_score = 0;
  int max = 0;
  for (int i = 0; i < max_clients; ++i) {
    if (clients[i].score > max_score) {
      max_score = clients[i].score;
      max = i;
    }
  }
  return max;
}

//Pass userlist and the index of the winner
void winner(const Client *clients, const int max_clients, const int index) {
  char msg[100] = "WINNER ";
  char num[20] = "";
  sprintf(num, "%d", clients[index].score);
  strcat(msg, num);
  strcat(msg, " ");
  strcat(msg, clients[index].name);
  strcat(msg, "\r\n");
  //TODO fix this
  for (int i = 0; i < max_clients; ++i) {
    if (clients[i].sock != 0) {
      write(clients[i].sock, msg, BUFFER_LEN);
    }
  }
}

//Starts the scrabble server
int scrabble_server(const char *ip, const int port, const int max_clients) {
  //Initialize client list
  Client *clients = (Client*) calloc(max_clients, sizeof(Client));

  //Initialize Board
  Tile **board = (Tile**) calloc(BOARD_HEIGHT, sizeof(Tile*));
  for (int i = 0; i < BOARD_HEIGHT; ++i)
    board[i] = (Tile*) calloc(BOARD_WIDTH, sizeof(Tile));

  //Initialize tiles on board
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      board[i][j].letter = '0';
    }
  }

  //Buffer
  char *buffer = (char*) calloc(BUFFER_LEN+1, sizeof(char));

  int sock, index, len, connected_clients;

  //Start server
  if ((sock = setup_server(ip, port, max_clients)) == -1)
    return -1;

  //Wait for people to connect
  while(true) {
    //Wait for output
    connected_clients = get_output(sock, clients, max_clients, &index, &buffer, &len);

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
      else if (strncmp(buffer, "winner", 6) == 0) {
        index = get_winner(clients, max_clients);
        winner(clients, connected_clients, index);
      }
      //TODO testing
      else if (strncmp(buffer, "push", 4) == 0) {
        board_push(clients, connected_clients, board);
      }
      else {
        if (check_command(buffer, len)) {
          char msg[100] = "We are okay";
          ok(&clients[index], msg, strlen(msg));
        }
        else {
          char msg[100] = "Things are bad";
          nok(&clients[index], msg, strlen(msg));
        }
      }

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
