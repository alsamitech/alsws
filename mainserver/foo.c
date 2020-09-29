#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int portnum = 1243;

int main(int argc, char *argv[]){
  int sockfd, newsockfd;
  struct sockaddr_in serv_addr, cli_addr; 
  sockfd = socket(AF_INET, SOCK_STREAM,0);
  if (sockfd < 0)  {
    printf("Can't Open Socket.\n");
    exit(1);
  }
  serv_addr.sin_family =  AF_INET;
  serv_addr.sin_port = htons(portnum);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  //Bind to the socket idk too much about this so I hope this compiles sucsessfully
  int res = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (res < 0) {printf("Can't bind socket.\n");exit(1);}
  // listen for connection whoohoo we actually might get a vistior on this site!!!
  listen(sockfd, 5);
  //accept the connection ill add dos protection later
  int clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
  //hand off to function

}
