/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlucie <hlucie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 11:26:29 by ehautefa          #+#    #+#             */
/*   Updated: 2022/06/09 16:54:21 by hlucie           ###   ########.fr       */
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

std::map<std::string, Channel>::iterator	Server::get_channel()
{
	std::map<std::string, Channel>::iterator	it = this->_channels.begin();
	while (it != this->_channels.end())
    {
		std::cout << it->first << std::endl;
		it++;
	}
	return (it);
}

std::vector<User>::iterator	Server::get_user(int fd)
{
	std::vector<User>::iterator	it = this->_users.begin();
	while (it != this->_users.end())
	{
		if (it->get_fd() == fd)
			return (it);
		it++;
	}
	return (it);
}

std::vector<User>::iterator	Server::get_user(std::string nickname)
{
	std::vector<User>::iterator	it = this->_users.begin();
	while (it != this->_users.end())
	{
		if (it->get_nickName().compare(nickname) == 0)
			return (it);
		it++;
	}
	return (it);
}

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

// COMMANDS



void	Server::user(std::vector<User>::iterator user, std::string username) {
	if (username == "")
		return ;
	std::cout << GR << "USER" << NC << std::endl;
	std::vector<std::string> tab = split(username, ' ');
	if (username.size() == 0 || tab.size() < 4) {
		user->send_message(to_string(ERRNEEDMOREPARAMS), ":Not enough parameters");
		return ;
	}
	if (user->get_nickName().compare(tab[0]) == 0 && user->get_hostName().compare(tab[2]) == 0) {
		user->set_userName(tab[1]);
		std::string	fullname = "";
		for (size_t i = 3; i < tab.size(); i++) {
			fullname += tab[i];
			if (i != tab.size() - 1)
				fullname += " ";
		}
		user->set_fullName(fullname.erase(0, 1));
	} else {
		user->send_message(to_string(ERRALREADYREGISTERED), ":Unauthorized command (already registered)");
		return ;
	}
	if (user->get_isConnected() == false) {
		user->set_isConnected(true);		
		user->send_message(to_string(RPL_WELCOME), user->get_nickName() + " :Welcome to the Internet Relay Network " + user->get_nickName() + "!"+ user->get_userName() +"@"+ user->get_hostName() +"\r\n");
	}
	user->print_user();
}

void	Server::nick(std::vector<User>::iterator user, std::string nickname) {
	if (nickname == "")
		return ;
	std::cout << GR << "NICK" << NC << std::endl;
	if (this->get_user(nickname)->get_fd() == 0) {
		user->send_message(to_string(ERRNICKNAMEINUSE), nickname + " :Nickname is already in use.");
		return ;
	}
	user->set_nickName(nickname);
}

void	Server::ping(std::vector<User>::iterator user, std::string server) {
	if (server == "")
		return ;
	std::cout << GR << "PING" << NC << std::endl;
	if (server.compare("localhost") != 0)
		user->send_message(to_string(ERRNOSUCHSERVER), server + " :No such server");
	else if (user->get_isConnected() == false)
		user->send_message(to_string(ERRNOORIGIN), ":No origin specified");
	else {
		std::string buf = "PONG " + server + user->get_nickName() + "\r\n";
		send(user->get_fd(), buf.c_str(), buf.size(), 0);
	}
}

int	Server::isInStr(std::string toFind, std::string channelName)
{
	int i = 0;
	int j = 1;
	while (toFind[i] != '\0')
	{
		while (channelName[j] != '\0')
		{
			if (toFind[i] == channelName[j] || channelName[j] == 7)
				return (1);
			j++;
		}
		j = 1;
		i++;
	}
	return (0);
}

void	Server::join(std::vector <User>::iterator user, std::string channel) {
	if (channel == "")
		return ;
	std::cout << GR << "JOIN" << NC << std::endl;
	if (channel.size() == 0) {
		user->send_message(to_string(ERRNEEDMOREPARAMS), ": Need more params");
		return ;
	}
	else if (channel.size() > 200 || channel[0] != '#' || channel[0] != '&'
			|| this->isInStr(" ,", channel))
	{
		user->send_message(to_string(ERRBADCHANMASK), ": WRONG PARAMS");
		return ;		
	}
	else
		this->_channels[channel] = Channel(channel);
	this->get_channel();
}

void	Server::whois(std::vector<User>::iterator user, std::string who) {
	if (who == "")
		return ;
	std::cout << GR << "WHOIS" << NC << std::endl;
	if (who.size() == 0) {
		user->send_message(to_string(ERRNONICKNAMEGIVEN), ":No nickname given");
		return ;
	}
	std::vector<User>::iterator to_ret = this->get_user(who);
	if (to_ret->get_fd() == 0) {
		user->send_message(to_string(ERRNOSUCHNICK), who + " :There was no such nickname");
		return ;
	}
	user->send_message(to_string(RPL_WHOISUSER), to_ret->get_nickName() + " " + to_ret->get_userName() + " " + to_ret->get_hostName() + " * :" + to_ret->get_fullName());
}


// METHODS

void	Server::parse_packets(char *packets, int fd) {
	std::vector<User>::iterator user = this->get_user(fd);
	
	this->nick(user, this->getInfo("NICK", std::string(packets)));
	this->user(user, this->getInfo("USER", std::string(packets)));
	this->ping(user, this->getInfo("PING", std::string(packets)));
	this->whois(user, this->getInfo("WHOIS", std::string(packets)));
	this->join(user, this->getInfo("JOIN", std::string(packets)));
}

std::string Server::getInfo(std::string to_find, std::string buffer)
{
	size_t begin = buffer.find(to_find);
	if (to_find[begin - 1] && to_find[begin - 1] != '\n')
		return ("");
	size_t end = buffer.find("\r\n", begin);
	if (begin == std::string::npos || begin < 0 || end == std::string::npos || end < 0 || begin >= end)
		return "";
	begin += to_find.size() + 1;
	end = begin;
	while (buffer[end] && buffer[end] != '\r' && buffer[end] != '\n')
		end++;
	std::string ret = buffer.substr(begin, end - begin);
	
	return ret;
}

void	Server::server_loop() {
	int	num_events;

	_pfds.push_back(pollfd());
	_pfds.back().events = POLLIN;
	_pfds.back().fd = _sockfd;
	while (1) {
	    std::cout << CYN << "SERVER: waiting for connections..." << NC << std::endl;
		num_events = poll(&_pfds[0], _pfds.size(), -1);
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
				_users.push_back(User(new_fd, hostname));
				std::cout << GRN << "SERVER: new connection from " << hostname << " on socket " << new_fd << NC << std::endl;
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
				_users.erase(this->get_user(_pfds[i].fd));
			}
			std::string pass = this->getInfo("PASS", std::string(packets));
			if (pass.size() > 0  && pass.compare(_password) != 0){
				std::cerr << RED << "ERROR: Wrong password" << NC << std::endl;
				close(_pfds[i].fd);
				_pfds.erase(_pfds.begin() + i);
				_users.erase(this->get_user(_pfds[i].fd));
			}
			else {
				std::cout << GRN << "RECEIVE: " << packets << NC << std::endl;
				this->parse_packets(packets, _pfds[i].fd);
			}
			for (int j = 0; packets[j] != '\0'; j++)
				packets[j] = '\0';
        }
    }
}