#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

/* The purpose of this small program is to illustrate the basics of Linux sockets */

int main()
{
	// creating socket
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "Socket: " << clientSocket << '\n';

	// specifying address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(3490);
	serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	// sending connection request
	connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	// sending data
	const char* message = "Hello, server!";
	send(clientSocket, message, strlen(message), 0);

	// closing socket
	close(clientSocket);
	std::cout << "Closed" << '\n';

	return 0;
}
