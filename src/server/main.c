#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../scrabble.h"
#include "../net.h"

#define ADDR "0.0.0.0"
#define PORT 9000
#define MAX_CLIENTS 8

int main(int argc, char *argv[]) {
  int port = PORT;
  if (argc > 1) {
    if (is_digit(argv[1]) && port > 0 && port < 65536) {
      port = atoi(argv[1]);
    }
    else {
      fprintf(stderr, "Must be a valid port");
      return -1;
    }
  }

  scrabble_server(ADDR, port, MAX_CLIENTS);

  return 0;
}
