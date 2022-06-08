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
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "../include/User.hpp"
#include "../include/Colors.hpp"

#define PASSWORD "12345"
#define LEN_MAX_PACKETS 512
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

std::string getIfnfo(std::string to_find, std::string buffer)
{
	size_t begin = buffer.find(to_find);
	size_t end = buffer.find("\r\n", begin);
	std::cout << "begin : " << begin << " end : " << end << std::endl;
	if (begin == std::string::npos || begin < 0 || end == std::string::npos || end < 0 || begin >= end)
		return "";
	std::cout << "begin : " << begin << " end : " << end << std::endl;
	begin += to_find.size() + 1;
	end = begin;
	while (buffer[end] && buffer[end] != ' ')
		end++;
	std::cout << "begin : " << begin << " end : " << end << std::endl;
	std::string ret = buffer.substr(begin, end - begin);
	std::cout << "ret : " << ret << std::endl;
	return ret;
}
 
int receive(std::vector<struct pollfd> pfds) {
    std::cout << CYN << "Check receive box ..." << NC << std::endl;
    for (size_t i = 1; i < pfds.size(); i++) {
        if (pfds[i].revents == POLLIN) {
            char packets[LEN_MAX_PACKETS];
            int size = recv(pfds[i].fd, packets, LEN_MAX_PACKETS, MSG_WAITALL);
            if (size == -1)
                std::cerr << RED << "ERROR: recv() failed" << NC << std::endl;
            std::cout << GRN << "RECEIVE: " << packets << NC << std::endl;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
	int                         sockfd, num_events;
	std::vector<struct pollfd>  pfds;

	if (check_arg(argc, argv) == false || (sockfd = init_socket(argv[1])) < 0)
		return EXIT_FAILURE;
	pfds.push_back(pollfd());
	pfds.back().events = POLLIN;
	pfds.back().fd = sockfd;
	while (1) {
	    std::cout << GRN << "SERVER: waiting for connections..." << NC << std::endl;
		num_events = poll(&pfds[0], pfds.size(), -1);
		std::cout << CYN << "SERVER: poll() returned " << num_events << " events" << NC << std::endl;
		if (num_events == -1) {
			std::cout << RED << "ERROR: POLL failed" << NC << std::endl;
		} else {
            if (pfds[0].revents == POLLIN) {
	            struct sockaddr_in  their_addr; // connector's address information
	            socklen_t           sin_size = sizeof(their_addr);
                int                 new_fd = accept(pfds[0].fd, (struct sockaddr *)&their_addr, &sin_size);
                if (new_fd == -1)
                    std::cerr << RED << "ERROR: accept failed" << NC << std::endl;
                char hostname[NI_MAXHOST];
                fcntl(new_fd, F_SETFL, O_NONBLOCK);
                if (getnameinfo((struct sockaddr *)&their_addr, sin_size, hostname, NI_MAXHOST, NULL, 0,  NI_NUMERICSERV) != 0)
                    std::cerr << RED << "ERROR : getnameinfo() failed" << NC << std::endl;
                pfds.push_back(pollfd());
                pfds.back().events = POLLIN;
                pfds.back().fd = new_fd;
                std::cout << GRN << "ADD USER WITH HOSTNAME : " << CYN << hostname << NC << std::endl;
            }
            else {
                receive(pfds);
            }
		}
	}
	return 0;
}