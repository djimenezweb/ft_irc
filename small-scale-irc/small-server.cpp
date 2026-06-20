#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

#define PORT "3490"

/* The purpose of this small program is to illustrate the basics of Linux sockets */

int	main()
{
	struct addrinfo hints;
	struct addrinfo *res;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;			// IPv4
	hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets
	hints.ai_flags = AI_PASSIVE;		// localhost

	getaddrinfo(0, PORT, &hints, &res);
	
	int serv_socket;
	serv_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	bind(serv_socket, res->ai_addr, res->ai_addrlen);

	listen(serv_socket, 10);
	std::cout << "Server listening at port " << PORT << '\n';

	int new_client;
	new_client = accept(serv_socket, 0, 0);

	char buffer[512] = { 0 };
	recv(new_client, buffer, sizeof(buffer), 0);
	std::cout << "Message from client: " << buffer << '\n';

	close(serv_socket);
	std::cout << "Closed" << '\n';

	freeaddrinfo(res);

	return (0);
}
