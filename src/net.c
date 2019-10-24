#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "net.h"

//Checks if input is a digit
bool is_digit(const char n[]) {
  for (int i = 0; n[i] != 0; ++i) {
    if (!isdigit(n[i]))
      return false;
  }
  return true;
}

//Checks if input is a valid ipv4 address
bool is_ip(const char *ip) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
    return result != 0;
}

