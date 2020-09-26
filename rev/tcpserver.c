#include "common.h"
#include <strings.h>

int main(int argc, char **argv) {
  int           listenfd, connfd, n;
  struct sockaddr_in  serveraddr;
  uint8_t         buff[MAXLINE + 1];
  uint8_t         recvline[MAXLINE + 1];

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_n_die("socket error.");
  bzero(&serveraddr, sizeof(servaddr));
  serveraddr.sin_family   = AF_INET;
  serveraddr.sin_addr.s_addr  = htonl(INADDR_ANY);
  serveraddr.sin_port     = htons(SERVER_PORT);

  if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) <0)
    err_n_die("bind error.");
  if ((listen(listenfd, 10)) < 0)
    err_n_die("listen error.");
  for ( ; ; ) {
    struct sockaddr_in addr;
    socklen_t addr_len;

    printf("waiting for connection on port %d\n", SERVER_PORT);
    fflush(stdout);
    connfd = accept(listenfd, (SA *) NULL, NULL);

    memset(recvline, 0, MAXLINE);

    while ( (n=read(connfd, recvline, MAXLINE-1) ) > 0) {
      fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);

      if ()
    }
  }
}