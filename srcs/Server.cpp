/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Server.cpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: ehautefa <ehautefa@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/06/08 11:26:29 by ehautefa		  #+#	#+#			 */
/*   Updated: 2022/06/09 17:35:42 by ehautefa		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../include/Server.hpp"


std::vector<std::string> split(const std::string &chaine, char delimiteur)
{
	if (chaine.empty())
		return std::vector<std::string>();
	std::vector<std::string> elements;
	std::stringstream ss(chaine);
	std::string sousChaine;
	while (std::getline(ss, sousChaine, delimiteur))
	{
		elements.push_back(sousChaine);
 	}
	 return elements;
}

// CONSTRUCTOR

Server::Server(int port, std::string password) : _port(port), _password(password), _pfds(), _users(), _channels(), _sockfd(0) {}

// DESTRUCTOR

Server::~Server() {
	std::cout << "Server is closing" << std::endl;
}

// SETTERS

bool	Server::set_sockfd(int sockfd) {
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
	while (it != this->_users.end()) {
		if (it->get_fd() == fd)
			return (it);
		it++;
	}
	return (it);
}

std::vector<User>::iterator	Server::get_user(std::string nickname) {
	std::vector<User>::iterator	it = this->_users.begin();
	while (it != this->_users.end()) {
		if (it->get_nickName().compare(nickname) == 0)
			return (it);
		it++;
	}
	return (it);
}

std::string 				Server::get_password() const { return (_password); }

int							Server::get_port() const { return (_port); }

std::vector<struct pollfd>	Server::get_pfds() const { return (_pfds); }

// std::map<std::string, Channel>	Server::get_channel() { return (this->_channels); }

std::vector<User>::iterator	Server::find_user(std::string str) {
    std::vector<User>::iterator	it = this->_users.begin();
    while (it != this->_users.end()) {
        if (it->get_nickName().compare(str) == 0)
            return (it);
        else if (str.compare(it->get_userName() + "@" + it->get_hostName()) == 0)
            return (it);
        else if (str.compare(it->get_nickName() + "!" + it->get_userName() + "@" + it->get_hostName()) == 0)
            return (it);
        it++;
    }
    return (it);
}

int	Server::isInStr(char toFind, std::string channelName)
{
	for (int i = 0; channelName[i]; i++) {
		if (toFind == channelName[i])
			return (1);
	}
	return (0);
}

// COMMANDS

void	Server::user(std::vector<User>::iterator user, std::pair<bool, std::string> username) {
	if (username.first == false)
		return ;
	std::cout << GR << "USER" << NC << std::endl;
	std::vector<std::string> tab = split(username.second, ' ');
	if (username.second.size() == 0 || tab.size() < 4) {
		user->send_error(to_string(ERRNEEDMOREPARAMS), ":Not enough parameters");
	} else if (!this->get_user(user->get_fd())->get_isConnected() == true) {
		user->set_userName(tab[1]);
		std::string	fullname = "";
		for (size_t i = 3; i < tab.size(); i++) {
			fullname += tab[i];
			if (i != tab.size() - 1)
				fullname += " ";
		}
		user->set_fullName(fullname.erase(0, 1));
	} else {
		user->send_error(to_string(ERRALREADYREGISTERED), ":Unauthorized command (already registered)");
	}
}

void	Server::nick(std::vector<User>::iterator user, std::pair<bool, std::string> nickname) {
	if (nickname.first == false)
		return ;
	std::cout << GR << "NICK" << NC << std::endl;
	if (nickname.second.size() == 0) {
		user->send_error(to_string(ERRNONICKNAMEGIVEN), ":No nickname given");
	} else if (nickname.second.size() > 9 || nickname.second.size() < 1
			|| nickname.second.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\`_^{|}0123456789-") != std::string::npos
			|| std::string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\`_^{|}").find(nickname.second[0]) == std::string::npos) {
		user->send_error(to_string(ERRERRONEUSNICKNAME), nickname.second + ":Erroneous nickname");
	} else if (user->get_mode('r') == true) {
		user->send_error(to_string(ERRRESTRICTED), ":You are restricted");
	} else if (this->get_user(nickname.second) != this->_users.end()) {
		user->send_other_error(to_string(ERRNICKNAMEINUSE), nickname.second + " :Nickname is already in use.");
		return ;
	} else {
		user->send_message("", "NICK :" + nickname.second);
		user->set_nickName(nickname.second);
	}
}
 
void	Server::ping(std::vector<User>::iterator user, std::pair<bool, std::string>  server) {
	if (server.first == false)
		return ;
	std::cout << GR << "PING" << NC << std::endl;
	if (server.second.compare("localhost") != 0)
		user->send_error(to_string(ERRNOSUCHSERVER), server.second + " :No such server");
	else if (user->get_isConnected() == false)
		user->send_error(to_string(ERRNOORIGIN), ":No origin specified");
	else {
		user->send_message("", "PONG " + server.second + " " + user->get_nickName() + "\r\n");
	}
}

void	Server::join(std::vector<User>::iterator user, std::pair<bool, std::string> channel) {
	if (channel.first == false)
		return ;
	std::cout << GR << "JOIN" << NC << std::endl;
	if (channel.second.size() == 0) {
		user->send_message(to_string(ERRNEEDMOREPARAMS), ": Need more params\r\n");
		return ;
	}
	else if (channel.second.size() > 200 || (channel.second[0] != '#' && channel.second[0] != '&')
			|| channel.second.find(' ', 0) != std::string::npos)
	{
		user->send_message(to_string(ERRBADCHANMASK), ": Wrong params\r\n");
		return ;
	}
	else if (_channels.count(channel.second) == 1)
		std::cout << "ALREADY EXIST" << std::endl;
	else {
		this->_channels[channel.second] = Channel(channel.second);
	}
	this->_channels[channel.second].users[user->get_nickName()] = *user;
	this->get_channel();
}

void	Server::whois(std::vector<User>::iterator user, std::pair<bool, std::string>  who) {
	if (who.first == false)
		return ;
	std::cout << GR << "WHOIS" << NC << std::endl;
	if (who.second.size() == 0) {
		user->send_error(to_string(ERRNONICKNAMEGIVEN), ":No nickname given");
		return ;
	}
	std::vector<User>::iterator to_ret = this->get_user(who.second);
	if (to_ret == this->_users.end()) {
		user->send_error(to_string(ERRNOSUCHNICK), who.second + " :There was no such nickname");
	} else
		user->send_message(to_string(RPL_WHOISUSER), to_ret->get_nickName() + " " + to_ret->get_userName() + " " + to_ret->get_hostName() + " * :" + to_ret->get_fullName());
}

void	Server::list(std::vector<User>::iterator user, std::pair<bool, std::string>  list) {
	if (list.first == false)
		return ;
	std::cout << GR << "LIST" << NC << std::endl;
	std::vector<std::string> tab = split(list.second, ' ');
	if ((tab.size() == 2 && !tab[1].empty() && tab[1].compare("localhost") != 0) || tab.size() > 2) {
		user->send_error(to_string(ERRNOSUCHSERVER), tab[1] + " :No such server");
	} else if (tab.size() == 1 || (tab.size() == 2 && tab[1].compare("localhost") == 0)) {
		std::vector<std::string> channel = split(tab[0], ',');
		for (size_t i = 0; i < channel.size(); i++) {
			if (this->_channels.count(channel[i]))
				user->send_message(to_string(RPL_LIST), channel[i] + " :" + this->_channels[channel[i]].getTopic());
		}
		user->send_message(to_string(RPL_LISTEND), ":End of LIST");
	} else if (tab.size() == 0) {
		std::map<std::string, Channel>::iterator it = this->_channels.begin();
		for(; it != this->_channels.end(); it++) {
			user->send_message(to_string(RPL_LIST), it->first + " :" + it->second.getTopic());
		}
		user->send_message(to_string(RPL_LISTEND), ":End of LIST");
	}
}

void	Server::oper(std::vector<User>::iterator user, std::pair<bool, std::string> oper) {
	if (oper.first == false)
		return ;
	std::cout << GR << "OPER" << NC << std::endl;
	std::vector<std::string> tab = split(oper.second, ' ');
	if (oper.second.size() == 0 || tab.size() < 2) {
		user->send_error(to_string(ERRNEEDMOREPARAMS), "OPER :Not enough parameters");
	} else if (this->_users.size() == 0) {
		user->send_error(to_string(ERRNOOPERHOST), ":No O-lines for your host");
	} else {
		std::vector<User>::iterator it = this->_users.begin();
		for (; it != this->_users.end(); it++) {
			if (it->get_userName().compare(tab[0]) == 0) {
				if (this->get_password().compare(tab[1]) == 0) {
					user->set_isOperator(true);
					user->send_message(to_string(RPL_YOUREOPER), ": You are now an IRC operator");
				} else {
					user->send_error(to_string(ERRNOOPERHOST), ":Password incorrect");
				}
				return ;
			}
		}
	}
}

bool	Server::die(std::vector<User>::iterator user, std::pair<bool, std::string> die) {
	if (die.first == false)
		return (false);
	std::cout << GR << "DIE" << NC << std::endl;
	if (user->get_isOperator() == false) {
		user->send_error(to_string(ERRNOPRIVILEGES), ":Permission Denied- You're not an IRC operator");
		return false;
	}
	return true;
}

void    Server::privmsg(std::vector<User>::iterator user, std::pair<bool, std::string> str) {
    if (str.first == false)
        return ;
    std::cout << GR << "PRIVMSG" << NC << std::endl;
    std::vector<std::string> tab = split(str.second, ' ');
	std::string	msg;
    if (tab.size() < 2) {
        user->send_error(to_string(ERRNOTEXTTOSEND), ":No text to send");
        return ;
    }
    std::vector<User>::iterator user_dest = this->find_user(tab[0]);
    std::map<std::string, Channel>::iterator chan_dest = _channels.find(tab[0]);
	msg = tab[1];
	for (size_t i = 2; i < tab.size(); i++) {
		msg += " " + tab[i];
	}
	msg.erase(0, 1);
	std::cout << YEL << "MESSAGE TO SEND :" << msg << NC << std::endl;
	if (user_dest != this->_users.end()) {
		if (user_dest->get_mode('a') == true)
			user->send_message(to_string(RPL_AWAY), user_dest->get_nickName() + " :" + user_dest->get_away());
		else
			user_dest->relay_message(*user, "PRIVMSG " + user_dest->get_nickName() + " :" + msg);
	} else if (chan_dest != this->_channels.end()) {
		chan_dest->second.send_message(*user, "PRIVMSG " + chan_dest->first + " :" + msg, false);
	} else if (user_dest == this->_users.end() && chan_dest == this->_channels.end()) {
        user->send_error(to_string(ERRNOSUCHNICK), tab[0] + " :No such nick/channel");
    } 
}

void	Server::mode(std::vector<User>::iterator user, std::pair<bool, std::string> mode) {
	if (mode.first == false)
		return ;
	std::cout << GR << "MODE" << NC << std::endl;
	std::vector<std::string> tab = split(mode.second, ' ');
	std::vector<User>::iterator user_dest = this->get_user(tab[0]);

	if (tab.size() < 2) {
		user->send_error(to_string(ERRNEEDMOREPARAMS), ":Not enough parameters");
	} else if (user_dest == this->_users.end()) {
		user->send_error(to_string(ERRNOSUCHNICK), tab[0] + " :No such nick");
	} else if (user_dest->get_nickName().compare(user->get_nickName()) != 0) {
		user->send_error(to_string(ERRUSERSDONTMATCH), ":Cannot change mode for other users");
	} else if (tab[1].size() != 2 || (tab[1][0] != '+' && tab[1][0] != '-')
		|| (tab[1].find_first_not_of("iwoOr+-") != std::string::npos)) {
		user->send_error(to_string(ERRUMODEUNKNOWNFLAG), ":Unknown MODE flag");
	} else {
		user->set_mode(tab[1][1]);
		user->send_message(to_string(RPL_UMODEIS), "i");
	}
}

void	Server::part(std::vector<User>::iterator user, std::pair<bool, std::string> part) {
	if (part.first == false)
		return ;
	std::string msg = "Part Message";
	std::cout << GR << "PART" << NC << std::endl;
	std::vector<std::string> tab = split(part.second, ' ');
	if (tab.size() < 1) {
		user->send_error(to_string(ERRNEEDMOREPARAMS), ":Not enough parameters");
		return ;
	} else if (tab.size() > 1) {
		msg = tab[1];
		for (size_t i = 2; i < tab.size(); i++) {
			msg += " " + tab[i];
		}
		msg.erase(0, 1);
	}
	std::vector<std::string> tab_chan = split(tab[0], ',');
	std::map<std::string, Channel>::iterator chan_dest;
	for (size_t i = 0; i < tab_chan.size(); i++) {
		chan_dest = this->_channels.find(tab[0]);
		if (chan_dest == this->_channels.end()) {
			user->send_error(to_string(ERRNOSUCHCHANNEL), tab[0] + " :No such channel");
		} else if (chan_dest->second.users.find(user->get_nickName()) == chan_dest->second.users.end()) {
			user->send_error(to_string(ERRNOTONCHANNEL), tab[0] + " :You're not on that channel");
		} else {
			chan_dest->second.send_message(*user, "PART " + tab_chan[i] + " :" + msg, false);
			chan_dest->second.users.erase(user->get_nickName());
		}
	}
}


// METHODS

bool	Server::parse_packets(char *packets, int fd) {
	std::vector<User>::iterator user = this->get_user(fd);
	
	this->nick(user, this->getInfo("NICK", std::string(packets)));
	this->user(user, this->getInfo("USER", std::string(packets)));
	this->ping(user, this->getInfo("PING", std::string(packets)));
	this->whois(user, this->getInfo("WHOIS", std::string(packets)));
	this->list(user, this->getInfo("LIST", std::string(packets)));
	this->oper(user, this->getInfo("OPER", std::string(packets)));
	this->privmsg(user, this->getInfo("PRIVMSG", std::string(packets)));
	this->join(user, this->getInfo("JOIN", std::string(packets)));
	this->mode(user, this->getInfo("MODE", std::string(packets)));
	this->part(user, this->getInfo("PART", std::string(packets)));
	if (user->get_isConnected() == false && user->get_nickName().size() != 0 && user->get_userName().size() != 0) {
		user->set_isConnected(true);		
		user->send_message(to_string(RPL_WELCOME), user->get_nickName() + " :Welcome to the Internet Relay Network " + user->get_nickName() + "!"+ user->get_userName() +"@"+ user->get_hostName());
		user->print_user();
	}
	return (this->die(user, this->getInfo("die", std::string(packets))));
}

std::pair<bool, std::string> Server::getInfo(std::string to_find, std::string buffer) {
	size_t begin = buffer.find(to_find);
	if (to_find[begin - 1] && to_find[begin - 1] != '\n')
		return (std::make_pair(false, ""));
	size_t end = buffer.find("\r\n", begin);
	if (begin == std::string::npos || end == std::string::npos || begin >= end)
		return (std::make_pair(false, ""));
	begin += to_find.size() + 1;
	end = begin;
	while (buffer[end] && buffer[end] != '\r' && buffer[end] != '\n')
		end++;
	std::string ret = buffer.substr(begin, end - begin);
	
	return std::make_pair(true, ret);
}

void	Server::server_loop() {
	int	num_events;
	bool stop = false;

	_pfds.push_back(pollfd());
	_pfds.back().events = POLLIN;
	_pfds.back().fd = _sockfd;
	while (!stop) {
		std::cout << CYN << "SERVER: waiting for connections..." << NC << std::endl;
		num_events = poll(&_pfds[0], _pfds.size(), -1);
		if (num_events == -1) {
			std::cout << RED << "ERROR: POLL failed" << NC << std::endl;
		} else {
			if (_pfds[0].revents == POLLIN) {
				struct sockaddr_in  their_addr; // connector's address information
				socklen_t		   sin_size = sizeof(their_addr);
				int				 new_fd = accept(_pfds[0].fd, (struct sockaddr *)&their_addr, &sin_size);
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
				stop = this->receive();
			}
		}
	}
}

bool	Server::receive() {
	bool	stop = false;
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
			std::pair<bool, std::string> pass = this->getInfo("PASS", std::string(packets));
			if (pass.first == true  && pass.second.compare(_password) != 0){
				std::cerr << RED << "ERROR: Wrong password" << NC << std::endl;
				close(_pfds[i].fd);
				_pfds.erase(_pfds.begin() + i);
				_users.erase(this->get_user(_pfds[i].fd));
			}
			else {
				std::cout << GRN << "RECEIVE: " << packets << NC << std::endl;
				stop = this->parse_packets(packets, _pfds[i].fd);
			}
			for (int j = 0; packets[j] != '\0'; j++)
				packets[j] = '\0';
		}
	}
	return (stop);
}