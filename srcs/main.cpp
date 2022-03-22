/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehautefa <ehautefa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 17:06:48 by ehautefa          #+#    #+#             */
/*   Updated: 2022/03/22 17:25:30 by ehautefa         ###   ########.fr       */
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

#define PASSWORD 	"12345"
#define BACKLOG 10   // how many pending connections queue will hold

std::string	itoa(int n) {
	std::stringstream out;
	out << n;
	return(out.str());
}

bool	check_arg(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << RED << "Attempt format is \"./ircserv <port> <password>\"" << NC << std::endl;
		return false;
	}
	std::string	port = argv[1];
	std::string	passWord = argv[2];
	for (unsigned int i = 0; port[i] != '\0'; i++) {
		if (!std::isdigit(port[i])) {
			std::cerr << RED << "Error : wrong format input" << NC << std::endl;
			return false;
		}
	}
	if (passWord != PASSWORD) {
		std::cerr << RED << "Error : wrong password input" << NC << std::endl;
		return false;
	}
	return true;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int	init_socket(char *port) {
	int		sockfd, rv;
    int 	yes = 1;
    struct	addrinfo hints, *servinfo, *p;
	
	memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            perror("server: socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            return -1;
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo); // all done with this structure
    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return -1;
    }
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        return -1;
    }
	return (sockfd);
}

int	main(int argc, char **argv)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];

    if (check_arg(argc, argv) == false || (sockfd = init_socket(argv[1])) < 0)
		return EXIT_FAILURE;

    printf("server: waiting for connections...\n");
	std::vector<struct pollfd>	pfds;
	
	struct pollfd pfds_tmp; // More if you want to monitor more
	pfds_tmp.events = POLLIN | POLLOUT;

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s %d\n", s, new_fd);

		pfds_tmp.fd = new_fd;          // Standard input
		pfds.push_back(pfds_tmp);


		printf("Hit RETURN or wait 2.5 seconds for timeout\n");

		int num_events = poll(&pfds[0], pfds.size(), 2500); // 2.5 second timeout

		if (num_events == 0) {
			printf("Poll timed out!\n");
		} else {
			int pollout_happened = pfds[0].revents & POLLOUT;
			int pollin_happened = pfds[0].revents & POLLIN;

			if (pollin_happened) {
				char	buffer[1024];
				recv(new_fd, buffer, 1024, 0);
				printf("RECEIVE : %s\n", buffer);
			}
			if (pollout_happened) {
				std::string message = "A message from server !";
				send(new_fd , message.c_str(), message.size(), 0 );
				printf("Server : Message has been sent ! \n");
			} else {
				printf("Unexpected event occurred: %d\n", pfds[0].revents);
			}
		}
    }

    return 0;
}