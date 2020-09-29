#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <eerno.h>
#include <unistd.h>

const int portnum = 1243;

void do_hof(int sock) {
  const int NOT_AUTH = 0;
  char buf[500];
  //strcpy(buf, "Got it!\n");

  //send(sock, buf, strlen(buf), 0);
  // send
  strcpy(buf, "250 hi!\n");
  send(sock, buf, strlen(buf), 0);

  int state = NOT_AUTH;

  while (1) {
    // Wait for input from user 
    recv(sock, buf, 1000 0);
    if(!strncmp("NAUTH", buf, 4)) {
      close(sock);
      return;
    }
    else if (!strncmp("MEIS", buf, 4) {
      char *username[995];
      sscanf(buf + 5, "%s ", username);
      printf("User is %s\n", username);
      // Send back confirmation
      strcpy(buf, "200 Logged in\n");
      send(sock, buf, 1000, 0);
    } else if (!strncmp("LAUS", buf, 4)) {
      // Lists all users
      strcpy(buf, "All Users\n");
    } else if (!strncmp("ALSO", buf, 4)) {

    } else if (!strncmp("ALSY", buf, 4)) {

    } else {
      // Send Error Message
      strcpy(buf, "500 - AUTH FAILED\n");
      send(sock, buf, strlen(buf), 0);
    }
  }

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
  if (res < 0) {perror("Can't bind socket.%s\n", perror(errno));exit(1);}
  // listen for connection whoohoo we actually might get a vistior on this site!!!
  listen(sockfd, 5);
  //accept the connection ill add dos protection later
  int clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
  //hand off to function
  do_hof(newsockfd);
  close(sockfd);

  return 0;
}
