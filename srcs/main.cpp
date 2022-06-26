/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.cpp										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: ehautefa <ehautefa@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/03/22 17:06:48 by ehautefa		  #+#	#+#			 */
/*   Updated: 2022/06/08 13:51:35 by ehautefa		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "../include/User.hpp"
#include "../include/Colors.hpp"
#include "../include/Server.hpp"

#define BACKLOG 10 // how many pending connections queue will hold

int init_socket(char *port)
{
	int sockfd, rv;
	int yes = 1;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		std::cerr << RED << "ERROR: GETADRRINFO failed" << NC << std::endl;
		return -1;
	}
	for (p = servinfo; p != NULL; p = p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1){
			std::cerr << RED << "ERROR: SOCKET failed" << NC << std::endl;
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1){
			std::cerr << RED << "ERROR: SETSOCKOPT failed" << NC << std::endl;
			return -1;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
			close(sockfd);
			std::cerr << RED << "ERROR: BIND failed" << NC << std::endl;
			continue;
		}
		break;
	}
	freeaddrinfo(servinfo);
	if (p == NULL){
		std::cerr << RED << "ERROR: Failed to bind" << NC << std::endl;
		return -1;
	}
	if (listen(sockfd, BACKLOG) == -1) {
		std::cerr << RED << "ERROR: LISTEN Failed" << NC << std::endl;
		return -1;
	}
	return (sockfd);
}

int main(int argc, char **argv)
{
	if (argc != 3){
		std::cerr << RED << "Attempt format is \"./ircserv <port> <password>\"" << NC << std::endl;
		return EXIT_FAILURE;
	} else if (std::string(argv[2]).empty()) {
		std::cerr << RED << "Password can't be empty" << NC << std::endl;
		return EXIT_FAILURE;
	}
	std::string port = argv[1];
	for (unsigned int i = 0; port[i] != '\0'; i++) {
		if (!std::isdigit(port[i])){
			std::cerr << RED << "Error : wrong format input" << NC << std::endl;
			return EXIT_FAILURE;
		}
	}
	Server	serv(atoi(argv[1]), argv[2]);
	
	if (serv.set_sockfd(init_socket(argv[1])) == false)
		return EXIT_FAILURE;
	serv.server_loop();
	return 0;
}