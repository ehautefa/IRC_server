#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"
#define GR "\e[0;90m"
#define BL "\e[0;30m"

#include <iostream>
#include <cctype>
#include <exception>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PASSWORD 	"12345"
#define BACKLOG		3

std::string	itoa(int n) {
	std::stringstream out;
	out << n;
	return(out.str());

}

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

int		init_info_addr(int port, struct addrinfo *address) {
	struct addrinfo hints;
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

	if (getaddrinfo(NULL, itoa(port).c_str() , &hints, &address) != 0) {
        std::cerr << RED << "Error : getaddrinfo failed" << NC << std::endl;
        return -1;
    }
	return 0;

// 	 getsockname, getprotobyname,
// gethostbyname, getaddrinfo, freeaddrinfo
}


int	main(int argc, char **argv)
{
	int		sock_fd, sock;
	int 	yes = 1;
	int 	port = check_arg(argc, argv);
	struct sockaddr_in addr;
	struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof their_addr;
	char buffer[1024] = {0};
	std::string message = "A message from server !";
	struct	addrinfo	*address = NULL;
	struct addrinfo hints;
	
	// memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

	if (getaddrinfo(NULL, itoa(port).c_str() , &hints, &address) != 0) {
        std::cerr << RED << "Error : getaddrinfo failed" << NC << std::endl;
        return -1;
    }
	std::cout << address->ai_family << std::endl;
	std::cout << address->ai_socktype << std::endl;
	std::cout << address->ai_flags << std::endl;
	std::cout << address->ai_protocol << std::endl;


	
	// if (port < 0 || init_info_addr(port, address) < 0)
	// 	return EXIT_FAILURE;
	address->ai_protocol = 0;
	sock_fd = socket ( address->ai_family, address->ai_socktype, address->ai_protocol);
	if (sock_fd == 0) {
		perror ( "Opening of Socket Failed !");
		return EXIT_FAILURE;
	}
	if ( setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {
		perror ( "Can't set the socket" );
		return EXIT_FAILURE;
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons( port ); //  convert values between host and network byte order
	if (bind(sock_fd, ( struct sockaddr * )&addr, sizeof(addr)) < 0) {
		perror ( "Binding of socket failed !" );
		return EXIT_FAILURE;
	}
	if (listen ( sock_fd, BACKLOG) < 0) {
		perror ( "Can't listen from the server !");
		return EXIT_FAILURE;
	}
	if ((sock = accept(sock_fd, (struct sockaddr *)&address, &addr_size)) < 0) {
		perror("Accept");
		return EXIT_FAILURE;
	}
	recv(sock_fd, buffer, 1024, MSG_WAITALL);
	printf("%s\n", buffer);
	send(sock , message.c_str(), message.size(), 0 );
	printf("Server : Message has been sent ! \n");
	
}


// int	main(int argc, char **argv)
// {
// 	int port = check_arg(argc, argv);
// 	int sock_fd, sock, reader;
// 	struct sockaddr_in address;
// 	int opted = 1;
// 	int address_length = sizeof(address);
// 	char buffer[1024] = {0};
// 	std::string message = "A message from server !";

// 	if (port < 0)
// 		return EXIT_FAILURE;
// 	if (( sock_fd = socket ( AF_INET, SOCK_STREAM, 0)) == 0) {
// 		perror ( "Opening of Socket Failed !");
// 		return EXIT_FAILURE;
// 	}
// 	if ( setsockopt(sock_fd, IPPROTO_TCP, SO_REUSEADDR, &opted, sizeof ( opted ))) {
// 		perror ( "Can't set the socket" );
// 		return EXIT_FAILURE;
// 	}
// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons( port ); //  convert values between host and network byte order
// 	if (bind(sock_fd, ( struct sockaddr * )&address, address_length) < 0) {
// 		perror ( "Binding of socket failed !" );
// 		return EXIT_FAILURE;
// 	}
// 	if (listen ( sock_fd, 3) < 0) {
// 		perror ( "Can't listen from the server !");
// 		return EXIT_FAILURE;
// 	}
// 	if ((sock = accept(sock_fd, (struct sockaddr *)&address, (socklen_t*)&address_length)) < 0) {
// 		perror("Accept");
// 		return EXIT_FAILURE;
// 	}
// 	reader = recv(sock_fd, buffer, 1024, MSG_WAITALL);
// 	printf("%s\n", buffer);
// 	send(sock , message.c_str(), message.size(), 0 );
// 	printf("Server : Message has been sent ! \n");
// 	return 0;
// }