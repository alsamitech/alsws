#include <stdio.h>
#include <stdlib.h>


// types and socket stuff
#include <sys/types.h>
#include <sys/socket.h>

#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

// stuff to do because I can

// IP and protcol stuff
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

/*
 *	Sami Alameddine, 2020
 *
 * 	This is another web server implementation by me
 * 	
 *
 * 	<executioncommand><binaryname> <portno> <file>
 * */

// defines
#define SA struct sockaddr
#define MAXLINE 4096

// configuration defines

#define ALS_DEBUG_TARGET

// Debug boilerplate

static FILE *FN;
void handler(int handlerNum);
#ifdef ALS_DEBUG_TARGET
char *bin2hex(const unsigned char *input, size_t len){
	char *db_res;
	char *hexits="0123456789ABCDEF";
	if(input==NULL||len<=0){
		return NULL;
	}
	int res_db_len=(len+3)+1;
	
	db_res=malloc(res_db_len);
	bzero(db_res,res_db_len);

	for(int db_i=0;db_i<len;db_i++){
		db_res[db_i*3]	=hexits[input[db_i]>>4];
		db_res[(db_i*3)+1]	=hexits[input[db_i]&0x0F];
		db_res[(db_i*3)+2]	=' ';
	}
	return db_res;
}
#endif

int main(int argc, char** argv) {
	signal(SIGINT, handler);
	int PORT_NO=6967;
	//int PORT_NO=argv[1];
	//char filenm=argv[2];
	char filenm[]="index.html";
	// declare stuff
	int  listenfd,connfd,n;
	struct sockaddr_in servaddr;
	uint8_t buff[MAXLINE+1];
	uint8_t recvline[MAXLINE+1];
	
	// allocates resources for an internet socket (a TCP socket)
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0){
		fprintf(stderr,"Error While trying too create socket");
		exit(1);
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=		AF_INET;
	// just says that it would responds to anything
	servaddr.sin_addr.s_addr=	htonl(INADDR_ANY);
	servaddr.sin_port=		htons(PORT_NO);
	
	// listen and bind
	if((bind(listenfd,(SA *)&servaddr,sizeof(servaddr)))<0){
		fprintf(stderr,"Error on bind");
		exit(1);
	}
	if((listen(listenfd,10))<0){
		fprintf(stderr, "Error on listen");
		exit(1);
	}

	FN=fopen("log","w+");

	FILE *FILEN;
	FILEN=fopen(filenm, "r");
	fseek(FILEN,0,SEEK_END);
	long fsize=ftell(FILEN);
	fseek(FILEN,0,SEEK_SET);
	
	//char *webpage=malloc(fsize+1);
	char *webpage[fsize+1];
	fread(webpage,1,fsize,FILEN);
		
	// sprintfs the thing

	//char webpage[]="HTTP/1.1 200 OK\r\n\r\nHello";

	fclose(FILEN);

	// infinite loop for handling requests
	for(;;){
		struct sockaddr_in addr;
		socklen_t addr_len;
		
		// accept blocks until an incomming connection arrives
		// it returns a "file descriptor" to the connection
		fprintf(stdout,"Waiting for a connection on port %d\n", PORT_NO);
		fflush(stdout);
		connfd=accept(listenfd, (SA *) NULL, NULL);

		// bzero out the recive buffer to make sure it ends up null terminated
		memset(recvline,0,MAXLINE);
		// read the client's message
		n=read(connfd,recvline,MAXLINE-1);
		fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline,n), recvline);
		fprintf(FN, "\n%s\n\n%s", bin2hex(recvline,n), recvline);
		// detects the end of a message
		if(n<0){fprintf(stderr,"cannot read what the client said! did the client send anything?");}
		snprintf((char*)buff, sizeof(buff),"HTTP/1.1 200 OK\r\n\r\n%s\r\n",webpage);
			
		//write(connfd,(char*)buff, strlen((char*)buff));
		write(connfd,(char*)buff,(strlen(webpage)+(21-1)));
		close(connfd);
	}
	fclose(FN);
}

void handler(int handlerNum) {
	fclose(FN);
	exit(2);
}
