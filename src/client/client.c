#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define REMOTE_ADDR "127.0.0.1"
#define REMOTE_PORT 1337

int main(int argc, char *argv[])
{
    struct sockaddr_in sa = {};
    int s;

    sa.sin_family = AF_INET;
    sa.sin_port = htons(REMOTE_PORT);
    sa.sin_addr.s_addr = inet_addr(REMOTE_ADDR);

    s = socket(AF_INET, SOCK_STREAM, 0);
    int c = connect(s, (struct sockaddr*) &sa, sizeof(sa));
    close(s);
    return 0;
}
