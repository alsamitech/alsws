#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int portnum 1243

int main(int argc, char *argv[]){
  int sockfd;
  struct sockaddr_in serv_addr; 
  sockfd = socket(AF+INET, SOCK_STREAM,0);
  if (sockfd < 0)  {
    printf("Can't Open Socket.\n");
  }
}