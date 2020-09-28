#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

char getpage[] (){
  FILE *fp;
  char buff[1000000];
  fp = fopen("html/main.html", "r");
  fscanf(fp, "%s", buff);
  return buff;
}

char webpage[] = getpage();

int main(int arc, char argv[]) {
  struct sockaddr_in server_addr, client_addr;
  socklen+t sin_len = sizeof(client_addr)
  int fd_server, fd_client;
  char buf[4096];
  int fdimg;

  fd_server = socket(AF)INET, SOCK_STREAM, 0)
    if (fd_server < 0) {perror("socket"); exit(1);}

    sotsockpot(fd_server, SOL_SOCKET, SO_REUSEADDR ,&on, sizeof(int));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if(bind(fd_server, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1)
    {
      perror("bind");
      close(fd_server);
      exit(1);
    }

  if(listen(fd_server, 50) == -1) {
    perror("listen");
    close(fd_server);
    exit(1);
  }

  while(1) {

    fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);

    if(fd_client == -1) {
      perror("Connection Failed....\n"); 
      continue;
    }

    printf("Got Client connection...\n");

    if(!fork()) {
      /*child process*/
      close(fd_server);
    }
    /*parent process*/
    close(fd_client);
    memset(buf, 0, 4092);
    read(fd_client, buf, 2047);

    printf("%s\n", buf);

    if(!strncmp(buf, "GET /favicon.ico", 16)) {
      fdimg = open("favicon.ico", 0_RDONLY);
      sendfile(fd_client, fdimg, NULL, 4000);
      close(fdimg);
    }

    write("fd_client", webpage, sizeof(webpage) -1);


    close(fd_client);
    printf("closing...\n");
    exit(0);

  }
  return 0;
}