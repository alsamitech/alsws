/*
 *		(c) Sami Alameddine, 2020
 *		
 *		The Alsami Project and ALSWS are owned by Sami Alameddine, and are maintained by Sami Alameddine and the community
 *
 *		This is a HTTP server that accepts command line arguments, has error handling, and has no loose numbers, making it easier to confgiure
 *
 *    To make this HTTP Server easier to study, almost every line that is not basic C code or boilerplate has a comment explaining what it's trying to
 *    
 *
 * */

#include <stdio.h>
#include <stdlib.h>
// This is the multithread header file for POISIX MULTITHREADING
#include <pthread.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "func.h"


#define CLIENTS_ALLOWED 20
#define FAIL_EXIT_NUM 1     
// Loose Numbers replaced with defines here because I don't want Yin making fun of me anymore
#define ret return

/* HTTP STATUS CODES */
#define OK 200                
// The HTTP 200 OK success status response code indicates that the request has succeeded. A 200 response is cacheable by default.
#define MOVEDPERMANENTLY 301  
//The HyperText Transfer Protocol (HTTP) 301 Moved Permanently redirect status response code indicates that the resource requested has been definitively moved to the URL given by the Location headers. A browser redirects to this page and search engines update their links to the resource.
#define BADREQUEST 400        
// The 400 Bad Request response status code indicates that the server cannot or will not process the request due to something that is perceived to be a client error (e.g., malformed request syntax, invalid request message framing, or deceptive request routing).
#define FORBIDDEN 403         
// The 403 Forbidden response status indicates that the server understands the request but refuses to authorize it
#define NOTFOUND 404          
// The HTTP 404 Not Found client error response code indicates that the server can't find the requested resource. 
#define GONE 410              
// The HyperText Transfer Protocol (HTTP) 410 Gone client error response code indicates that access to the target resource is no longer available at the origin server and that this condition is likely to be permanent.
#define METHODNOTALLOWED 405  
// The HyperText Transfer Protocol (HTTP) 405 Method Not Allowed response status code indicates that the request method is known by the server but is not supported by the target resource.
#define UNSUPPORTEDMEDIATYPE 415
#define HTTPVERSIONNOTSUPPORTED 505       
// If it is using the wrong HTTP version that the server does not suppot, the server will respond with this
/*Status Code Documentation adapted from Mozilla*/

char hostname[];

void *ALS_HDLPRC(){
  return NULL;
}

int main(int argc, char argv[]) {
	
	// Parse the command line args
	/*if(argc && *argv == NULL) {
		perror("Error: You Did not enter argc or argv!!!\nUsage: ./executable portnum file");		// Essentially, if argc and argv are NULL, that means that the user did not enter those arguments, so this if statment will check if it's null, if it is, it would print out the error message and exit with the value specified in the FAIL_EXIT_NUM define
		exit(FAIL_EXIT_NUM);
	} else if(argc == NULL) {
		perror("Error: You did not enter argc\nArgument Usage: ./executable portnum file");		// Same thing as before, but checks if argc (the portnumber) is null, and than prints out the usage of the command line args (just in case)
		exit(FAIL_EXIT_NUM);
	} else if(*argv == NULL) {
		perror("Error: You did not enter argv\nArgument Usage: ./executable portnum file");		// checks if argc is equal to NULL, if it is, it juust prints owt the error and dies
	} excluded for now*/
	int SERVER_PORT = argc;

  printf("Enter Hostname");
  scanf("%[^\n]", hostname);

	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client; // declares server variables

	char buf[2048];		// This stores whatever the client decides to send back to the server

	int fdimg;		// Holds general image media, ie favicons and images
	int on = 1;

	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server < 0) {
		// This if statements checks to see if fd_server is less than 0, if it is, something went wrong and we need to print out an error message.
		perror("socket");
		exit(FAIL_EXIT_NUM);
	}

	// Otherwise, if everything works, we have to set up the socket properties.
	setsockopt(fd_server, SOL_SOCKET,  SO_REUSEADDR, &on, sizeof(int));
	
	server_addr.sin_family = AF_INET;		// Essentially, this says that we want to communicate over the Internet Protocol Version 4 (IPv4) protocol.
	server_addr.sin_addr.s_addr = INADDR_ANY;		// Essentially means that the server will be bound to all local interfaces

	server_addr.sin_port = htons(SERVER_PORT);		// Tells what port we are going to use, WARINING, NEVER HAVE YOUR NUMBERS BE LOOSE, EIHTER PUT THEM IN A DEFINE OR A CONSTANT

	/*
	 *
	 * NOTE:
	 *	ON GNU/LINUX, IF YOU PICK A PORT BELOW 1024, YOU WILL NEED SUPERUSER PERMISSION
	 *	SO IF YOU SET SERVER_PORT TO BELOW 1024, PLEASE RUN THIS WHILE IN ROOT.
	 *
	 * */

	if(bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)/* THIS ELEMENT DOES THE BINDING PART, AND IF SOMETHING GOES WRONG, IT WILL ERR_N_DIE*/{
		perror("Error On Bind");		// Prints out error
		close(fd_server);			// Closes server
		exit(FAIL_EXIT_NUM);				// Exits program
	}

	if(listen(fd_server, CLIENTS_ALLOWED/*This server can handle up to 20 clients at once by default, but scince this is in a define, you can change it*/) == -1) {
		perror("Error on listen");		// Prints out error message
		close(fd_server);
		exit(FAIL_EXIT_NUM);				// Exists program with status code 1 (You can change this by changing the define)
	}

	while(1) {
	
		// This is where the action stats, this is the while loop that actually does the serving of pages
		fd_client = accept(fd_server, (struct sockaddr*) &client_addr, &sin_len);		// Waits for and accepts clients

		if (fd_client = -1) {
			perror("Connection Failed...\n");		// prints error message
			continue;		// While this is an error, it does not require the program to be stopped nescessarly 
		}

		printf("Got Client connection...\n");


		// The child process will return 0, the parent process would recive the child's process ID.
		if(!fork()) {
			/*child process*/
			close(fd_server);		// Closes fd_server because the child does not need it

			memset(buf, 0, 2048);		// CLears the buffer
			read(fd_client, buf, 2047);	// Read's the client's buffer

			printf("%s\n", buf);		// Pritns the content of the buffer, what the browser sent
      FILE *frc;

      frc = fopen("als.log", "w+");
      fprintf(frc, "ALSAMI SERVER LOG\n%s\n", buf);
      fclose(frc);

      FILE *f = fopen(argv, "rb");
      fseek(f, 0, SEEK_END);
      long fsize = ftell(f);
      fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

      char *webpage = malloc(fsize + 1);
      fread(webpage, 1, fsize, f);
      fclose(f);

      webpage[fsize] = 0;

			write(fd_client, webpage, sizeof(webpage) - 1);		// sends the webpage to fd_client

			// -1 to remove one of the null icons
		}
		/*parent process*/
		close(fd_client);
	
	}

	ret 0;
}
