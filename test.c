#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int buffer_len = 20;
  char *buffer = (char*) malloc((buffer_len+1)*sizeof(char));
  memset(buffer, 0, buffer_len);

  buffer = "Hi there!";
  printf("%s\n", buffer);
  free(buffer);
  return 0;
}
