#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <netinet/in.h>

int main() {
  // create a socket
  int NET_SOCK;
  NET_SOCK = socket(AF_INET, SOCK_STREAM, 0);


  // specity an address for the socket
  struct sockaddr_in server_adress;
  server_adress.sin_family = AF_INET;
  server_adress.sin_port = htons(9550);
  server_adress.sin_addr.s_addr = INADDR_ANY;

  int CN_RES = connect(NET_SOCK, (struct sockaddr *) &server_adress, sizeof(server_adress));
  // check for error with the connection
  if (CN_RES == -1) {
    printf("There was an error making a connection with to the remote socket \n\n");
  }

  char server_response[256];

  // recive data from the server
  recv(NET_SOCK, &server_response, sizeof(server_response), 0);
  
  //print out the server's response

  printf("The server sent the data %s\n", server_response);

  // and then close the socket
  close(NET_SOCK);
  return 0;
}