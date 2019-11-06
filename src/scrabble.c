#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "scrabble.h"

#define AUTHOR "Ryan Stewart"
#define VERSION "1.0.1"

//Does a quick check to see if the command might be valid
//Yes I know this is a horrible looking function. This might get fixed in the future
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

//TODO fix this
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

//TODO fix this
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

//Checks if a user sets their name
bool is_userset(const char* msg, const int len) {
  if (strncmp(Command.userset, msg, strlen(Command.userset)) == 0) {
    for (int i = strlen(Command.userset); msg[i] != '\r' && msg[i+1] != '\n'; ++i) {
      if (i > len || i - strlen(Command.userset) > 32)
        return false;
    }
    return true;
  }
  return false;
}

//Sets the username
bool userset_server(Client *clients, const int connected_clients, const int index,
                    const char *name, const int len) {
  for (int i = 0; i < connected_clients; ++i) {
    if (strcmp(clients[i].name, name) == 0 && i != index)
      return false;
  }
  memset(clients[index].name, 0, NAME_LEN);
  memcpy(clients[index].name, name, len);
  clients[index].name_len = len;
  return true;
}

//Sets the username
void userset_client(Client *c, const char *name, const int len) {
  char msg[60] = "USERSET ";
  int msg_len = strlen(msg);
  memcpy(msg + msg_len, name, len);
  write(c->sock, msg, msg_len + len);
}

//Announces a user changes their name
void userchange(const Client *clients, const int connected_clients, const char *old_name,
                const int old_len, const char *name, const int len) {
  char msg[100] = "USERCHANGE ";
  int n = strlen(msg);
  memcpy(msg+n, old_name, old_len);
  n += old_len;
  memcpy(msg+n, " ", 1);
  ++n;
  memcpy(msg+n, name, len);
  n += len;
  memcpy(msg+n, "\r\n", 2);
  n += 2;
  for (int i = 0; i < connected_clients; ++i) {
    write(clients[i].sock, msg, n);
  }
}

//Sends an announcement to client that clients from the list have joined
void userjoin(Client *c, Client *clients, const int clients_len) {
  for (int i = 0; i < clients_len; ++i) {
    char msg[100] = "USERJOIN ";
    int n = strlen(msg);
    memcpy(msg+n, clients[i].name, clients[i].name_len);
    n += clients[i].name_len;
    memcpy(msg+n, "\r\n", 2);
    n += 2;
    write(c->sock, msg, n);
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

//Checks if the given string is a tile
bool is_tile(const char *s) {
  const char tile[6] = "(A,0)";
  for (int i = 0; i < 5; ++i) {
    if (i == 1) {
      if(s[i] != '0' && (s[i] - tile[i] < 0 || s[i] - tile[i] > 25))
        return false;
    }
    else if (i == 3) {
      if (s[i] < '0' || s[i] > '4')
        return false;
    }
    else if (s[i] != tile[i]) {
      return false;
    }
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

//Check if something is a board
bool is_board(const char *msg, const int len) {
  char* push = (char*) calloc(20, sizeof(char));
  char* tile = (char*) calloc(6, sizeof(char));
  strcpy(push, Command.boardpush);
  strcat(push, "\r\n");
  int n = strlen(push);
  //Make sure message starts with "BOARDPUSH "
  if (n < len && strncmp(push, msg, n) == 0) {
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
      for (int j = 0; j < BOARD_WIDTH; ++j) {
        //Copy in tile
        strncpy(tile, msg+n, 5);
        //Check if the tile is valid and free memory if not
        if(n+5 < len && !is_tile(tile)) {
          free(push);
          free(tile);
          return false;
        }
        //Advance 5
        n += 5;
      }
      //Check if the format still fits, free otherwise
      if (n+2 < len && strncmp(msg+n, "\r\n", 2) != 0) {
        free(push);
        free(tile);
        return false;
      }
      n += 2;
    }
  }
  free(push);
  free(tile);
  return true;
}

//Read in the board
//Uses memory efficiency over time efficiency
void read_board(Tile **board, const char *msg) {
  char* tile = (char*) calloc(6, sizeof(char));
  int n = strlen(Command.boardpush)+2;
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      //Copy tile in and copy it into the board
      strncpy(tile, msg+n, 5);
      str_to_tile(&board[i][j], tile);
      n += 5;
    }
    n += 2;
  }
  free(tile);
}

//TODO fix this
//Checks if is place
bool is_place(const char *msg, const int len) {
  const char place[10] = "(A,0,0)";
  char num[20] = "";
  int n = 0;
  int place_len = strlen(place), i;
  if (len > place_len && msg[0] == '(') {
    for (i = 0; i + 1 < len && msg[i+1] != '\r' && msg[i+2] != '\n'; ++i) {
      if (i%place_len == 1) {
        if(msg[i+1] != '0' && (msg[i+1] - place[i] < 0 || msg[i+1] - place[i] > 25))
          return false;
      }
      //TODO fix this
      else if (i%place_len == 3 || i%place_len == 5) {
        if (msg[i+1] < '0' || msg[i+1] > '4')
          return false;
      }
      else if (msg[i+1] != place[i]) {
        return false;
      }
    }
    if (i < len && strncmp(msg+i, ")\r\n", 3) == 0)
      return true;
    return false;
  }
  return false;
}

//TODO fix this
//Places a tile
void place(Client *c, const Tile *tiles, const int len) {
  char *msg = (char*) calloc(BUFFER_LEN+1, sizeof(char));
  char n[3] = "";
  strcat(msg, "(");
  for (int i = 0; i < len; ++i) {
    strcat(msg, "(");
    strcat(msg, &tiles[i].letter);
    strcat(msg, ",");
    sprintf(n, "%d", tiles[i].x);
    strcat(msg, n);
    memset(n,0,3);
    strcat(msg, ",");
    sprintf(n, "%d", tiles[i].y);
    strcat(msg, n);
    memset(n,0,3);
    strcat(msg, ")");
  }
  strcat(msg, ")\r\n");
  write(c->sock, msg, BUFFER_LEN);
  free(msg);
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

//TODO fix this
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
      hello(clients+index);
    }
    //Client sent message
    else if (buffer != NULL) {
      printf("%d %s", len, buffer);

      //Check to see if a command might be valid
      if (!check_command(buffer, len)) {
        char msg[100] = "Command does not exist!";
        nok((clients+index), msg, strlen(msg));
      }

      //Check the if hello is correct
      if (clients[index].hello && is_hello(buffer, len)) {
        char msg[100] = "Welcome! Please set a username";
        clients[index].hello = false;
        ok(clients+index, msg, strlen(msg));

        //Set client's name to their ip
        char ip[100] = "";
        strcat(ip, inet_ntoa(clients[index].sa.sin_addr));
        strcat(clients[index].name, ip);
        clients[index].name_len = strlen(ip);

        //Create new client list to send to
        if (connected_clients > 1) {
          Client *join = (Client*) calloc(connected_clients-1, sizeof(Client));
          int joined = 0;

          //Alert all old clients the new person has joined
          for (int i = 0; i < connected_clients; ++i) {
            if (i != index) {
              userjoin((clients+i), (clients+index), 1);
              join[joined] = clients[i];
              ++joined;
            }
          }
          //Alert the new client everyone else joined
          userjoin((clients+index), join, joined);
        }
      }
      else if (clients[index].hello && !is_hello(buffer, len)) {
        char msg[100] = "Sorry, that's not a valid greeting";
        nok(clients+index, msg, strlen(msg));
        goodbye(clients+index);
      }
      else if (is_quit(buffer, len)) {
        goodbye(clients+index);
      }
      else if (strncmp(Command.place, buffer, strlen(Command.place)) == 0) {
          if (is_place(buffer, len)) {
            printf("Here\n");
          }
          else  {
            char msg[100] = "Incorrect format";
            nok((clients+index), msg, strlen(msg));
          }
      }
      //Check to see if a user is trying to set their name
      else if (strncmp(Command.userset, buffer, strlen(Command.userset)) == 0) {
        char old_name[40] = "";
        memcpy(old_name, clients[index].name, clients[index].name_len);
        int old_len = clients[index].name_len;

        //See if the username is set
        if (is_userset(buffer, len)) {
          char name[40] = "";
          int n = 0;
          //Find length of string
          for (int i = strlen(Command.userset)+1; buffer[i] != '\r' && buffer[i] != '\n'; ++i,++n);
          memcpy(name, buffer+strlen(Command.userset)+1, n);
          //Try to successfully set username
          if (userset_server(clients, connected_clients, index, name, n)) {
            char msg[100] = "Name changed successfully";
            ok((clients+index), msg, strlen(msg));
            //Make sure there is a name to actually announce a change
            if (old_len > 0) {
              userchange(clients, connected_clients, old_name, old_len, name, n);
            }
          }
          else  {
            char msg[100] = "The given name is already taken";
            nok((clients+index), msg, strlen(msg));
          }
        }
        else  {
          char msg[100] = "Incorrect format";
          nok((clients+index), msg, strlen(msg));
        }
      }
      else if (strncmp(buffer, "winner", 6) == 0) {
        index = get_winner(clients, max_clients);
        winner(clients, connected_clients, index);
      }
      //TODO testing
      else if (strncmp(buffer, "push", 4) == 0) {
        board_push(clients, connected_clients, board);
      }
    }
    if (buffer == NULL)
      buffer = calloc(BUFFER_LEN+1, sizeof(char));
    else
      memset(buffer, 0, BUFFER_LEN);

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
