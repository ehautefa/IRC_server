/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehautefa <ehautefa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 11:26:29 by ehautefa          #+#    #+#             */
/*   Updated: 2022/06/08 11:59:18 by ehautefa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

// CONSTRUCTOR

Server::Server(int port, std::string password) : _port(port), _password(password), _pfds(), _users(), _sockfd(0)
{

}

// DESTRUCTOR

Server::~Server()
{
	std::cout << "Server is closing" << std::endl;
}

// SETTERS

bool	Server::set_sockfd(int sockfd)
{
	if (sockfd < 0)
		return (false);
	this->_sockfd = sockfd;
	return (true);
}

// GETTERS

std::string Server::get_password()
{
	return (this->_password);
}

int		Server::get_port()
{
	return (this->_port);
}

std::vector<struct pollfd> Server::get_pfds()
{
	return (this->_pfds);
}


// METHODS

void	Server::server_loop() {
	int	num_events;

	_pfds.push_back(pollfd());
	_pfds.back().events = POLLIN;
	_pfds.back().fd = _sockfd;
	while (1) {
	    std::cout << GRN << "SERVER: waiting for connections..." << NC << std::endl;
		num_events = poll(&_pfds[0], _pfds.size(), -1);
		std::cout << CYN << "SERVER: poll() returned " << num_events << " events" << NC << std::endl;
		if (num_events == -1) {
			std::cout << RED << "ERROR: POLL failed" << NC << std::endl;
		} else {
            if (_pfds[0].revents == POLLIN) {
	            struct sockaddr_in  their_addr; // connector's address information
	            socklen_t           sin_size = sizeof(their_addr);
                int                 new_fd = accept(_pfds[0].fd, (struct sockaddr *)&their_addr, &sin_size);
                if (new_fd == -1)
                    std::cerr << RED << "ERROR: accept failed" << NC << std::endl;
                char hostname[NI_MAXHOST];
                fcntl(new_fd, F_SETFL, O_NONBLOCK);
                if (getnameinfo((struct sockaddr *)&their_addr, sin_size, hostname, NI_MAXHOST, NULL, 0,  NI_NUMERICSERV) != 0)
                    std::cerr << RED << "ERROR : getnameinfo() failed" << NC << std::endl;
                _pfds.push_back(pollfd());
                _pfds.back().events = POLLIN;
                _pfds.back().fd = new_fd;
                std::cout << GRN << "ADD USER WITH HOSTNAME : " << CYN << hostname << NC << std::endl;
            } else {
                this->receive();
            }
		}
	}
}

void	Server::receive() {
	std::cout << CYN << "Check receive box ..." << NC << std::endl;
    for (size_t i = 1; i < _pfds.size(); i++) {
        if (_pfds[i].revents == POLLIN) {
            char packets[LEN_MAX_PACKETS];
            int size = recv(_pfds[i].fd, packets, LEN_MAX_PACKETS, MSG_WAITALL);
            if (size == -1)
                std::cerr << RED << "ERROR: recv() failed" << NC << std::endl;
			else if (size == 0) {
				std::cerr << RED << "ERROR: Client disconnected" << NC << std::endl;
				close(_pfds[i].fd);
				_pfds.erase(_pfds.begin() + i);
			}
            std::cout << GRN << "RECEIVE: " << packets << NC << std::endl;
        }
    }
}