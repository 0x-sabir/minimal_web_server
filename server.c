#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>

#define PORT 5001
#define BUFFER_SIZE 1024
#define QUEUE_SIZE 20
#define THREAD_POOL_SIZE 10

//Thread pool to handle incoming reqests
pthread_t thread_pool[THREAD_POOL_SIZE];

//Mutex to sync thread pool access
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg)
{
	int client_socket = *((int *)arg);
	free(arg);
	
	char buffer[BUFFER_SIZE];
	memset(buffer,  0, BUFFER_SIZE);

	read(client_socket, buffer, BUFFER_SIZE -1);
	printf("Recieved request: \n %s \n", buffer);

	const char* response =
         "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "Connection: close\r\n\r\n"
        "Response Generated from SERVER!";

	write(client_socket, response, strlen(response));


	close(client_socket);
	return NULL;


}



void *request_handler(void *arg)
{
	int server_fd = *((int *)arg);
	while(1)
	{
		struct sockaddr_in client_address;
		socklen_t client_len = sizeof(client_address);
		int *client_socket = malloc(sizeof(int));
		
		*client_socket = accept(server_fd,(struct sockaddr *)&client_address, &client_len);
		if(*client_socket < 0)
		{
			perror("Connection is NOT ACCEPTED!!!");
			free(client_socket);
			continue;
		}

		printf("Connection Accepted from %s:%d \n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
		
		pthread_mutex_lock(&mutex);
		pthread_t thread;

		if(pthread_create(&thread, NULL, handle_client, client_socket) != 0)
		{
		perror("thread creation failed!!!!");
		close(*client_socket);
		free(client_socket);

		} else {
			pthread_detach(thread);
		}
		
		pthread_mutex_unlock(&mutex);

	}
 	return NULL;

}

int main()
{

	int server_socket_fd ;
	struct sockaddr_in server_address;
	
	//Creating socket 
	server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket_fd < 0){
		perror("Failed to create socket!!");
		exit(EXIT_FAILURE);
	}


	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);


	//binding the server socket
	int bind_socket = bind(server_socket_fd,(struct sockaddr*)&server_address, sizeof(server_address));
	if(bind_socket < 0)
	{
	perror("Binding socket failed completely!!!");
	close(server_socket_fd);
	exit(EXIT_FAILURE);
	}

	//Listening for client connection or requests

	int listening = listen(server_socket_fd, QUEUE_SIZE);
	if(listening < 0)
	{
		perror("Listening FAILED!!");
		close(server_socket_fd);
		exit(EXIT_FAILURE);
	}
	else{
		printf(" Listening on port %d \n", PORT);
	}

	for(int i=0; i < THREAD_POOL_SIZE; i++)
	{
		if(pthread_create(&thread_pool[i],NULL, request_handler, &server_socket_fd) != 0 )
		{
			perror("Threadm creation FAILED!!!");
			close(server_socket_fd);
			exit(EXIT_FAILURE);
		}
	}


	for(int i = 0; i < THREAD_POOL_SIZE; i++)
	{
		pthread_join(thread_pool[i], NULL);
	}	

	close(server_socket_fd);
	pthread_mutex_destroy(&mutex);

	return 0;
}
