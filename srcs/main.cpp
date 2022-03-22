#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"
#define GR "\e[0;90m"
#define BL "\e[0;30m"

#include <iostream>
#include <string>
#include <cctype>
#include <exception>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#define PASSWORD "12345"

int check_arg(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << RED << "Attempt format is \"./ircserv <port> <password>\"" << NC << std::endl;
		return (-1);
	}
	std::string	port = argv[1];
	std::string	passWord = argv[2];
	for (unsigned int i = 0; port[i] != '\0'; i++) {
		if (!std::isdigit(port[i])) {
			std::cerr << RED << "Error : wrong format input" << NC << std::endl;
			return (-1);
		}
	}
	if (passWord != PASSWORD) {
		std::cerr << RED << "Error : wrong password input" << NC << std::endl;
		return (-1);
	}
	return (atoi(argv[1]));
}

int	set_addrinfo(struct addrinfo hints, struct addrinfo *serveinfo, char *port)
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_addr = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; // for TCP protoctol
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, port, &hints, &serveinfo))
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	int int_port = check_arg(argc, argv);
	int sock_fd, sock, reader;
	struct addrinfo		hints;
	struct addrinfo		*serveinfo = NULL;
	struct sockaddr_in	address;
	int opted = 1;
	int address_length = sizeof(address);
	char buffer[1024] = {0};
	std::string message = "A message from server !";

	if (int_port < 0 || set_addrinfo(hints, serveinfo, argv[1]))
		return 1;
	if (( sock_fd = socket ( AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror ( "Opening of Socket Failed !");
		exit ( EXIT_FAILURE);
	}
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof ( opted ))) {
		perror ( "Can't set the socket" );
		exit ( EXIT_FAILURE );
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( int_port ); //  convert values between host and network byte order
	if (bind(sock_fd, ( struct sockaddr * )&address, sizeof(address))<0)
	{
		perror ( "Binding of socket failed !" );
		exit(EXIT_FAILURE);
	}
	if (listen ( sock_fd, 3) < 0)
	{
		perror ( "Can't listen from the server !");
		exit(EXIT_FAILURE);
	}
	if ((sock = accept(sock_fd, (struct sockaddr *)&address, (socklen_t*)&address_length)) < 0)
	{
		perror("Accept");
		exit(EXIT_FAILURE);
	}
	reader = read(sock, buffer, 1024);
	printf("%s\n", buffer);
	send(sock , message.c_str(), message.size(), 0 );
	printf("Server : Message has been sent ! \n");
	return 0;
}