#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>			//For struct sockaddr_in
#include <strings.h> 			//For bzero()
#include <time.h>
#include <sys/times.h>
#include <errno.h>

#define MAX_CONNECTIONS		5
#define PORT 				8888
#define SIZE 				100

void main(){

	/* AF_INET    -> IPv4 Internet Protocols
	   SOCK_STRAM -> Provides sequenced, reliable, two-way, connection-based byte streams.
	                 An out-of-band data transmission mechanism may be supported.*/
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0){
		printf("Error in creating socket!!\n");
		exit(0);
	}

	printf("Socket created\n");

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 


	//Binds socket created and Addr to it
	if(bind(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0 ){
		printf("Binding failed \n");
		exit(0);
	}

	printf("Binding successful\n");

	//Listen for connections, and has the maximum of 5 connections
	if(listen(socketfd, MAX_CONNECTIONS) != 0){
		printf("Error in listening\n");
		exit(0);
	}

	printf("Server is now listening\n");

	struct sockaddr aux;
	time_t ticks;
	char buffer[SIZE];

	while(1){
		//Get the lenght of the struct for later
		
		int lenght = sizeof(aux);

		int acc = accept(socketfd, (struct sockaddr*)&servaddr, &lenght);
		if (acc < 0){
			printf("Client not accepted!\n");
			exit(0);
		}

		printf("Client accepted..\n");

		//get the time
		ticks = time(NULL);

		//storing inside buffer the message
		snprintf(buffer, sizeof(buffer), "%s\n", ctime(&ticks));
		write(acc, buffer, strlen(buffer));

		close(acc);
		sleep(1);

	}

	close(socketfd);

	return;
}