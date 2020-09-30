#include "common.h"
#include <strings.h>


char getpage () {
  FILE * fp;
  fp = fopen("html/main.html", "r");
  char sng[500];
  while (!feof(fp)) {
    fgets(sng, 500, fp);
  }
}

char wpx = getpage();

int main(int argc, char **argv) {
  int           listenfd, connfd, n;
  struct sockaddr_in  serveraddr;
  uint8_t         buff[MAXLINE + 1];
  uint8_t         recvline[MAXLINE + 1];

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_n_die("socket error.");
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family   = AF_INET;
  serveraddr.sin_addr.s_addr  = htonl(INADDR_ANY);
  serveraddr.sin_port     = htons(SERVER_PORT);

  if ((bind(listenfd, (SA *) &serveraddr, sizeof(serveraddr))) <0)
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

      if (recvline[n-1] == '\n') {
        break;
      }

      memset(recvline, 0, MAXLINE);
    }
    if (n < 0)
      err_n_die("read error");
    
    snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\n%s",wpx);
  }
}