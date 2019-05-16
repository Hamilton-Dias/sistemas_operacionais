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

pthread_mutex_t	lock;

void *func(void *socketfd){
	//converte de volta para int
	int newid = *((int *)socketfd);
	char caso;

	char buffer[SIZE];
	bzero(&buffer, sizeof(buffer));

	strcpy(buffer, "Digite a operação que deseja realizar:\n\t1.Criar (Sub)diretório\n\t2.Remover (Sub)diretório\n\t3.Entrar em (sub)diretório\n\t4.Mostrar conteúdo do diretório\n\t5.Criar arquivo\n\t6.Remover arquivo\n\t7.Escrever um sequência de caracteres em um arquivo\n\t8.mostrar conteúdo do arquivo\n\n");
    send(newid, buffer, strlen(buffer), 0);

	do{
		if (strncmp(buffer, "mkdir\n", 6) == 0){
			printf("teste\n");

			pthread_mutex_lock(&lock);
           	int check; 
		    char* dirname = "Nova Pasta"; 	  
		    check = mkdir(dirname); 		  
		    // check if directory is created or not 
		    if (!check) 
		        printf("Pasta Criada\n"); 
		    else  
		        printf("ERRO\n");  
		 
            pthread_mutex_unlock(&lock);				    	
		}	
		//Lendo mensagem e retornando a mensagem
			read(newid, buffer, sizeof(buffer));
		//resposta no servidor
			//printf("Mensagem recebida: %s\t", buffer);
	}while(strncmp(buffer, "exit\n", 5) != 0);

	return;
}



void main(){

	/* AF_INET    -> IPv4 Internet Protocols
	   SOCK_STRAM -> Provides sequenced, reliable, two-way, connection-based byte streams.
	                 An out-of-band data transmission mechanism may be supported.*/
	//Criando Socket
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0){
		printf("Error in creating socket!!\n");
		exit(0);
	}

	printf("Socket created\n");

	//Criando nome
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 
		    

	//Criando Bind
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

	//Cria o numero de threads com base no máximo de conexões
	pthread_t thread[MAX_CONNECTIONS];

	//contador para as threads
	int i=0;

	while(1){
		//Get the lenght of the struct for later
		
		int lenght = sizeof(aux);

		//ACCEPT('RECEBENDO O SOCKET','NOME DO SOCKET', TAMANHO DESSE SOCKET)
		int acc = accept(socketfd, (struct sockaddr*)&servaddr, &lenght);
		if (acc < 0){
			printf("Client not accepted!\n");
			exit(0);
		}

		printf("Client accepted..\n");

		//Coloca a thread para gerenciar a conexão
		pthread_create(&thread[i], NULL, func, (void*)&acc);

		pthread_join(thread[i], NULL);

		close(acc);
		sleep(1);

	}

	close(socketfd);

	return;
}