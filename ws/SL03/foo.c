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

#define SERVER_PORT 5002
#define CLIENTS_ALLOWED 20
#define FAIL_EXIT_NUM 1
#define ret return


int main(int argc, char argv[]) {
	

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
			perror("Connection Filed...\n");		// prints error message
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

      FILE *f = fopen("index.html", "rb");
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
