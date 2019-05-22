#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>			//For struct sockaddr_in
#include <strings.h> 			//For bzero()
#include <time.h>
#include <sys/times.h>
#include <errno.h>

#define MAX_CONNECTIONS		2
#define PORT 				8888
#define SIZE 				100

pthread_mutex_t	lock;

void *func(void *socketfd){
	//converte de volta para int
	int newid = *((int *)socketfd);
	char caso;

	char buffer[SIZE];
	bzero(&buffer, sizeof(buffer));

	strcpy(buffer, "Digite a operação que deseja realizar:\n\tmkdir - Criar (Sub)diretório\n\trmdir - Remover (Sub)diretório\n\tcd - Entrar em (sub)diretório\n\tls -l - Mostrar conteúdo do diretório\n\ttouch - Criar arquivo\n\trm - Remover arquivo\n\techo- Escrever um sequência de caracteres em um arquivo\n\t8.mostrar conteúdo do arquivo\n\n");
    send(newid, buffer, strlen(buffer), 0);

	while(1){
		

		if (strncmp(buffer, "mkdir ", 6) == 0){
            pthread_mutex_lock(&lock);
            system(buffer);
            printf("\nPasta criada\n");
            pthread_mutex_unlock(&lock);
        }

        if (strncmp(buffer, "rmdir ", 6) == 0){
            pthread_mutex_lock(&lock);
            system(buffer);
            printf("\nPasta excluida\n");
            pthread_mutex_unlock(&lock);
        }

        if (strncmp(buffer, "cd ", 3) == 0){
            pthread_mutex_lock(&lock);
            memmove(buffer, buffer + 3, strlen(buffer));
            chdir(buffer);
            printf("\nEntrando no diretorio %s\n", buffer);
            pthread_mutex_unlock(&lock);
         }

         if (strncmp(buffer, "ls ", 3) == 0){
            pthread_mutex_lock(&lock);
            system(buffer);
            pthread_mutex_unlock(&lock);
        }

        if (strncmp(buffer, "touch ", 6) == 0){
            pthread_mutex_lock(&lock);
            system(buffer);
            printf("\nArquivo criado\n");
            pthread_mutex_unlock(&lock);
        }

        if (strncmp(buffer, "rm ", 3) == 0){
            pthread_mutex_lock(&lock);
            system(buffer);            
            printf("Arquivo removido\n");
            pthread_mutex_unlock(&lock);
        }

        if (strncmp(buffer, "echo ", 5) == 0){
            pthread_mutex_lock(&lock);
            system(buffer);
            printf("Caracteres inseridos\n");
            pthread_mutex_unlock(&lock);
        }

        if (strncmp(buffer, "cat ", 4) == 0){
            pthread_mutex_lock(&lock);
            system(buffer);
            pthread_mutex_unlock(&lock);
        }

        if(strncmp(buffer, "exit\n", 5) != 0){
			pthread_mutex_lock(&lock);
            close(newid);
            pthread_mutex_unlock(&lock);		
        }

		//Lendo mensagem e retornando a mensagem
		bzero(&buffer, sizeof(buffer));
		read(newid, buffer, sizeof(buffer));
	}

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

		if (i >= MAX_CONNECTIONS){
			i = 0;
			while (i < MAX_CONNECTIONS){
		pthread_join(thread[i++], NULL);		
			}
			i =0;
		}
		

		
		sleep(1);

	}

	close(socketfd);

	return;
}