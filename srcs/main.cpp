/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehautefa <ehautefa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 17:06:48 by ehautefa          #+#    #+#             */
/*   Updated: 2022/03/22 17:50:03 by ehautefa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"
#define GR "\e[0;90m"
#define BL "\e[0;30m"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#define PASSWORD "12345"
#define BACKLOG 10 // how many pending connections queue will hold

std::string itoa(int n)
{
	std::stringstream out;
	out << n;
	return (out.str());
}

bool check_arg(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << RED << "Attempt format is \"./ircserv <port> <password>\"" << NC << std::endl;
		return false;
	}
	std::string port = argv[1];
	std::string passWord = argv[2];
	for (unsigned int i = 0; port[i] != '\0'; i++)
	{
		if (!std::isdigit(port[i]))
		{
			std::cerr << RED << "Error : wrong format input" << NC << std::endl;
			return false;
		}
	}
	if (passWord != PASSWORD)
	{
		std::cerr << RED << "Error : wrong password input" << NC << std::endl;
		return false;
	}
	return true;
}

int init_socket(char *port)
{
	int sockfd, rv;
	int yes = 1;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0)
	{
		std::cerr << RED << "ERROR: GETADRRINFO failed" << NC << std::endl;
		return -1;
	}
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1)
		{
			std::cerr << RED << "ERROR: SOCKET failed" << NC << std::endl;
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		{
			std::cerr << RED << "ERROR: SETSOCKOPT failed" << NC << std::endl;
			return -1;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			std::cerr << RED << "ERROR: BIND failed" << NC << std::endl;
			continue;
		}
		break;
	}
	freeaddrinfo(servinfo);
	if (p == NULL)
	{
		std::cerr << RED << "ERROR: Failed to bind" << NC << std::endl;
		return -1;
	}
	if (listen(sockfd, BACKLOG) == -1)
	{
		std::cerr << RED << "ERROR: LISTEN Failed" << NC << std::endl;
		return -1;
	}
	return (sockfd);
}

int main(int argc, char **argv)
{
	struct sockaddr_storage their_addr; // connector's address information
	int sockfd, new_fd, num_events;
	socklen_t sin_size;
	struct pollfd pfds_tmp;
	std::vector<struct pollfd> pfds;

	if (check_arg(argc, argv) == false || (sockfd = init_socket(argv[1])) < 0)
		return EXIT_FAILURE;
	std::cout << CYN << "SERVER: waiting for connections..." << NC << std::endl;
	pfds_tmp.events = POLLIN;
	pfds_tmp.fd = sockfd;
	pfds.push_back(pfds_tmp);
	while (1)
	{
		num_events = poll(&pfds[0], pfds.size(), -1);
		std::cout << GR << "SERVER: poll() returned " << num_events << " events" << NC << std::endl;
		if (num_events == -1){
			std::cout << RED << "ERROR: POLL failed" << NC << std::endl;
		}
		else {
			sin_size = sizeof their_addr;
			new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
			if (new_fd == -1)
				std::cerr << RED << "ERROR: accept failed" << NC << std::endl;

			pfds_tmp.fd = new_fd;
			pfds_tmp.events = POLLIN;
			pfds.push_back(pfds_tmp);

			if (pfds[0].revents & POLLIN) {
				char buffer[1024];
				recv(new_fd, buffer, 1024, 0);
				std::cout << GRN << "SERVER receive: " << buffer << NC << std::endl;
			}
		}
	}

	return 0;
}