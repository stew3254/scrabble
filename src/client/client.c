#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../scrabble.h"
#include "../net.h"

#define ADDR "127.0.0.1"
#define PORT 9000
#define BUFFER_LEN 4096

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
  char *buffer = (char*) calloc(BUFFER_LEN, sizeof(char*));
  Client c = scrabble_connect(ip, port);

  return 0;
}
