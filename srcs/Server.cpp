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

Server::Server(int port, std::string password) : _port(port), _password(password), _pfds(), _users(), _channels(), _bannedList(), _sockfd(0) {}

// DESTRUCTOR

Server::~Server() {
	std::cout  << "Server is closing" << std::endl;
}

// SETTERS

bool	Server::set_sockfd(int sockfd) {
	if (sockfd < 0)
		return (false);
	this->_sockfd = sockfd;
	return (true);
}

// GETTERS

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

// COMMANDS

void	Server::user(std::vector<User>::iterator user, std::pair<bool, std::string> username) {
	if (username.first == false)
		return ;
	std::cout  << GR << "USER" << NC << std::endl;
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
	std::cout  << GR << "NICK" << NC << std::endl;
	if (nickname.second.size() == 0) {
		user->send_error(to_string(ERRNONICKNAMEGIVEN), ":No nickname given");
	} else if (nickname.second.size() > 9 || nickname.second.size() < 1
			|| nickname.second.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\`_^{|}0123456789-") != std::string::npos
			|| std::string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\`_^{|}").find(nickname.second[0]) == std::string::npos) {
		user->send_error(to_string(ERRERRONEUSNICKNAME), nickname.second + ":Erroneous nickname");
	} else if (this->get_user(nickname.second) != this->_users.end()) {
		user->send_other_error(to_string(ERRNICKNAMEINUSE), nickname.second + " :Nickname is already in use.");
		return ;
	} else {
		if (user->get_nickName().size() > 0) {
			user->send_error("","NICK :" + nickname.second);
		} else
			user->send_message("", "NICK :" + nickname.second);
		user->set_nickName(nickname.second);
		std::map<std::string, Channel>::iterator it = this->_channels.begin();
		for (; it != this->_channels.end(); it++) {
			it->second.setNickname(user->get_fd(), user->get_nickName());
		}
	}
}
 
void	Server::ping(std::vector<User>::iterator user, std::pair<bool, std::string>  server) {
	if (server.first == false)
		return ;
	std::cout  << GR << "PING" << NC << std::endl;
	if (server.second.compare("localhost") != 0)
		user->send_error(to_string(ERRNOSUCHSERVER), server.second + " :No such server");
	else if (user->get_isConnected() == false)
		user->send_error(to_string(ERRNOORIGIN), ":No origin specified");
	else {
		user->send_message("", "PONG");
	}
}

void	Server::join(std::vector<User>::iterator user, std::pair<bool, std::string> channel, bool invite) {
	if (channel.first == false)
		return ;
	std::cout  << GR << "JOIN" << NC << std::endl;
	if (_channels.count(channel.second) == 1)
		std::cout << RED << "Mode Channel :" << _channels[channel.second].getMode() << " INVITE " << invite << std::endl;
	if (channel.second.size() == 0) {
		user->send_message(to_string(ERRNEEDMOREPARAMS), ":Need more params\r\n");
		return ;
	}
	else if (channel.second.size() > 200 || (channel.second[0] != '#' && channel.second[0] != '&')
			|| channel.second.find(' ', 0) != std::string::npos)
	{
		user->send_message(to_string(ERRBADCHANMASK), ":Wrong params\r\n");
		return ;
	} 
	else if (_channels.count(channel.second) == 1) {
		if (_channels[channel.second].getChannelMode('i') == true && invite == false) {
			user->send_message(to_string(ERRINVITEONLYCHAN), channel.second + " :Cannot join channel (Invite only)");
			return ;
		}
		if (_channels[channel.second].getKickStatus(user->get_fd()) == true) {
			user->send_message(to_string(ERRBANNEDFROMCHAN), channel.second);
			return ;
		}
		this->_channels[channel.second].addUser(*user, ' ');
	} else {
		this->_channels[channel.second] = Channel(channel.second);
		this->_channels[channel.second].addUser(*user, 'O');
	}
	this->_channels[channel.second].send_message(*user, "JOIN " + channel.second, true);
	std::cout  << this->_channels[channel.second].userIsOn() << std::endl;
	if (_channels[channel.second].getTopic() == "")
		user->send_message(to_string(RPL_NOTOPIC), channel.second + " :No topic is set");
	else
		user->send_message(to_string(RPL_TOPIC), user->get_nickName() + " " + channel.second + " :" + this->_channels[channel.second].getTopic());
	user->send_message(to_string(RPL_NAMREPLY), user->get_nickName() + " = " + channel.second + " :" + this->_channels[channel.second].userIsOn());
	user->send_message(to_string(RPL_ENDOFNAMES), user->get_nickName() + " " + channel.second + " :End of NAMES list");		
}

void	Server::whois(std::vector<User>::iterator user, std::pair<bool, std::string>  who) {
	if (who.first == false)
		return ;
	std::cout  << GR << "WHOIS" << NC << std::endl;
	if (who.second.size() == 0) {
		user->send_error(to_string(ERRNONICKNAMEGIVEN), ":No nickname given");
		return ;
	}
	std::vector<User>::iterator to_ret = this->get_user(who.second);
	if (to_ret == this->_users.end()) {
		user->send_error(to_string(ERRNOSUCHNICK), who.second + " :There was no such nickname");
	} else
		user->send_other_error(to_string(RPL_WHOISUSER), to_ret->get_nickName() + " " + to_ret->get_userName() + " " + to_ret->get_hostName() + " * :" + to_ret->get_fullName());
}

void	Server::list(std::vector<User>::iterator user, std::pair<bool, std::string>  list) {
	if (list.first == false)
		return ;
	std::cout  << GR << "LIST" << NC << std::endl;
	std::vector<std::string> tab = split(list.second, ' ');
	if ((tab.size() == 2 && !tab[1].empty() && tab[1].compare("localhost") != 0) || tab.size() > 2) {
		user->send_error(to_string(ERRNOSUCHSERVER), tab[1] + " :No such server");
	} else if (tab.size() == 1 || (tab.size() == 2 && tab[1].compare("localhost") == 0)) {
		std::vector<std::string> channel = split(tab[0], ',');
		for (size_t i = 0; i < channel.size(); i++) {
			if (this->_channels.count(channel[i]))
				user->send_other_error(to_string(RPL_LIST), channel[i] + " :" + this->_channels[channel[i]].getTopic());
		}
		user->send_other_error(to_string(RPL_LISTEND), ":End of LIST");
	} else if (tab.size() == 0) {
		std::map<std::string, Channel>::iterator it = this->_channels.begin();
		for(; it != this->_channels.end(); it++) {
			user->send_other_error(to_string(RPL_LIST), it->first + " :" + it->second.getTopic());
		}
		user->send_other_error(to_string(RPL_LISTEND), ":End of LIST");
	}
}

void	Server::oper(std::vector<User>::iterator user, std::pair<bool, std::string> oper) {
	if (oper.first == false)
		return ;
	std::cout  << GR << "OPER" << NC << std::endl;
	std::vector<std::string> tab = split(oper.second, ' ');
	if (oper.second.size() == 0 || tab.size() < 2) {
		user->send_error(to_string(ERRNEEDMOREPARAMS), "OPER :Not enough parameters");
	} else if (this->_users.size() == 0) {
		user->send_error(to_string(ERRNOOPERHOST), ":No O-lines for your host");
	} else {
		std::vector<User>::iterator it = this->_users.begin();
		for (; it != this->_users.end(); it++) {
			if (it->get_nickName().compare(tab[0]) == 0) {
				if (tab[1].compare(OPER_MDP) == 0) {
					user->set_isOperator(true);
					it->send_message(to_string(RPL_YOUREOPER), ": You are now an IRC operator");
				} else {
					user->send_error(to_string(ERRNOOPERHOST), ":Password incorrect");
				}
				return ;
			} 
		}
		if (it == this->_users.end()) {
			user->send_error(to_string(ERRNOSUCHNICK), tab[0] + " :No such nick");
		}
	}
}

bool	Server::die(std::vector<User>::iterator user, std::pair<bool, std::string> die) {
	if (die.first == false)
		return (false);
	std::cout  << GR << "DIE" << NC << std::endl;
	if (user->get_isOperator() == false) {
		user->send_error(to_string(ERRNOPRIVILEGES), ":Permission Denied- You're not an IRC operator");
		return false;
	}
	return true;
}

void    Server::privmsg(std::vector<User>::iterator user, std::pair<bool, std::string> str) {
    if (str.first == false)
        return ;
    std::cout  << GR << "PRIVMSG" << NC << std::endl;
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
	if (user_dest != this->_users.end()) {
		user_dest->relay_message(*user, "PRIVMSG " + user_dest->get_nickName() + " :" + msg);
	} else if (chan_dest != this->_channels.end()) {
		if (chan_dest->second.users.find(user->get_fd()) == chan_dest->second.users.end()) 
			user->send_error(to_string(ERRNOTONCHANNEL), tab[0] + " :You're not on that channel");
		else if (chan_dest->second.getChannelMode('m') == false || chan_dest->second.isVoice(user->get_fd()))
			chan_dest->second.send_message(*user, "PRIVMSG " + chan_dest->first + " :" + msg, false);
	} else if (user_dest == this->_users.end() && chan_dest == this->_channels.end()) {
        user->send_error(to_string(ERRNOSUCHNICK), tab[0] + " :No such nick/channel");
    } 
}

void	Server::mode(std::vector<User>::iterator user, std::pair<bool, std::string> mode) {
	if (mode.first == false)
		return ;
	std::cout  << GR << "MODE" << NC << std::endl;
	std::vector<std::string> tab = split(mode.second, ' ');

	if (tab[0][0] == '#' || tab[0][0] == '&') {
		// C'est un channel
		if (tab.size() < 1) {
			user->send_error(to_string(ERRNEEDMOREPARAMS), ":Not enough parameters");
			return ;
		}
    	std::map<std::string, Channel>::iterator chan_dest = _channels.find(tab[0]);
		if (chan_dest == _channels.end())
			user->send_message(to_string(ERRNOSUCHCHANNEL), tab[0] + " :No such channel");
		else if (chan_dest->second.users.find(user->get_fd()) == chan_dest->second.users.end())
			user->send_error(to_string(ERRNOTONCHANNEL), tab[0] + " :You're not on that channel");
		else {
			if (tab.size() == 1) {
				if (chan_dest->second.getChannelMode().size() == 0)
					user->send_error(to_string(RPL_CHANNELMODEIS), user->get_nickName() + " " + tab[0]);
				else
					user->send_error(to_string(RPL_CHANNELMODEIS), user->get_nickName() + " " + tab[0] + " " + chan_dest->second.getChannelMode());
			} else if (tab.size() == 2) {
				if (tab[1].compare("b") == 0)
					return ;
				if (chan_dest->second.isOperator(user->get_fd()) == false) {
					user->send_message(to_string(ERRCHANOPRIVSNEED), " :You're not an operator");
				} else if (tab[1][0] == '+' || tab[1][0] == '-') {
					for (size_t i = 1; i < tab[1].size(); i++) {
						if (tab[1][i] == 'i' || tab[1][i] == 'm')
							if (tab[1][0] == '+')
								chan_dest->second.setChannelMode(tab[1][i]);
							else
								chan_dest->second.deleteChannelMode(tab[1][i]);
						else
							user->send_other_error(to_string(ERRUMODEUNKNOWNFLAG), std::string(1, tab[1][i]) + " :is unknown mode char to me for " + tab[0]);
					}
					chan_dest->second.send_message(*user, to_string(RPL_CHANNELMODEIS) + " " + user->get_nickName() + " " + tab[0] + " " + chan_dest->second.getChannelMode(), true);
				} else if (tab[1].size() == 1 && tab[1][0] == 'O'){
					user->send_message(to_string(RPL_UNIQOPIS), tab[0] + " " + chan_dest->second.getCreator());
				} else {
					user->send_error(to_string(ERRUMODEUNKNOWNFLAG), ":Unknown MODE flag");
				}
			} else if (tab.size() == 3) {
				if (chan_dest->second.isOperator(user->get_fd()) == false) {
					user->send_message(to_string(ERRCHANOPRIVSNEED), " :You're not an operator");
				} else if (chan_dest->second.users.find(this->get_user(tab[2])->get_fd()) == chan_dest->second.users.end()) {
						user->send_error(to_string(ERRUSERNOTINCHANNEL), " :User not in channel");
				} else if (tab[1].size() == 2 && (tab[1][0] == '+' || tab[1][0] == '-')
					&& (tab[1][1] == 'o' || tab[1][1] == 'v')) {
						if (tab[1][0] == '+')
							chan_dest->second.set_userMode(this->get_user(tab[2])->get_fd(), tab[1][1]);
						else
							chan_dest->second.delete_userMode(this->get_user(tab[2])->get_fd(), tab[2][1]);
						
					this->get_user(tab[2])->send_message(to_string(RPL_UMODEIS), " MODE : in channel " + chan_dest->second.getName() + "  " + std::string(1, chan_dest->second.getUserMode(this->get_user(tab[2])->get_fd())));
				} else
					user->send_other_error(to_string(ERRUMODEUNKNOWNFLAG), std::string(1, tab[1][1]) + " :is unknown mode char to me for " + tab[0]);
			} else {      
				user->send_error(to_string(ERRUMODEUNKNOWNFLAG), ":Unknown MODE flag");
			}
		}
	} else { // change USER MODE (not channel)
		if (tab.size() < 2) {
			user->send_error(to_string(ERRNEEDMOREPARAMS), ":Not enough parameters");
		}
		std::vector<User>::iterator user_dest = this->get_user(tab[0]);
		if (user_dest == this->_users.end()) {
			user->send_error(to_string(ERRNOSUCHNICK), tab[0] + " :No such nick");
		} else if (user_dest->get_nickName().compare(user->get_nickName()) != 0) {
			user->send_error(to_string(ERRUSERSDONTMATCH), " :Cannot change mode for other users");
		} else if (tab[1].size() != 2 || (tab[1][0] != '+' && tab[1][0] != '-')
			|| tab[1][1] != 'i') {
			user->send_error(to_string(ERRUMODEUNKNOWNFLAG), " :Unknown MODE flag");
		} else {
			if (tab[1][0] == '+')
				user->set_mode(tab[1][1]);
			else
				user->delete_mode('i');
			if (user->get_isOperator() == true)
				user->send_message(to_string(RPL_UMODEIS), " MODE :" + user->get_mode() + "o");
			else 
				user->send_message(to_string(RPL_UMODEIS), " MODE :" + user->get_mode());
		}
	}
}

void	Server::part(std::vector<User>::iterator user, std::pair<bool, std::string> part) {
	if (part.first == false)
		return ;
	std::string msg = "Part Message";
	std::cout  << GR << "PART" << NC << std::endl;
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
		} else if (chan_dest->second.users.find(user->get_fd()) == chan_dest->second.users.end()) {
			user->send_error(to_string(ERRNOTONCHANNEL), tab[0] + " :You're not on that channel");
		} else {
			chan_dest->second.send_message(*user, "PART " + tab_chan[i] + " :" + msg, true);
			chan_dest->second.users.erase(user->get_fd());
			if (chan_dest->second.users.size() == 0)
				this->_channels.erase(chan_dest);
		}
	}
}

void	Server::topic(std::vector<User>::iterator user, std::pair<bool, std::string> topic) {
	if (topic.first == false)
		return ;
	std::cout  << GR << "TOPIC" << NC << std::endl;
	std::vector<std::string> tab = split(topic.second, ' ');
	if (tab.size() < 1) {
		user->send_error(to_string(ERRNEEDMOREPARAMS), ":Not enough parameters");
		return ;
	}
	std::map<std::string, Channel>::iterator chan_dest = this->_channels.find(tab[0]);
	if (chan_dest == this->_channels.end()) {
		user->send_error(to_string(ERRNOSUCHCHANNEL), tab[0] + " :No such channel");
	} else if (chan_dest->second.users.find(user->get_fd()) == chan_dest->second.users.end()) {
		user->send_error(to_string(ERRNOTONCHANNEL), tab[0] + " :You're not on that channel");
	} else if (chan_dest->second.isOperator(user->get_fd()) == false) {
		user->send_error(to_string(ERRCHANOPRIVSNEED), tab[1] + " :You're not channel operator");
		return ;
	} else {
		if (tab.size() == 1 && chan_dest->second.getTopic().empty()) {
			user->send_message(to_string(RPL_NOTOPIC), tab[0] + " :No topic is set");
		} else  if (tab.size() == 1) {
			user->send_message(to_string(RPL_TOPIC), "TOPIC " + tab[0] + " :" + chan_dest->second.getTopic());
		} else {
			for (size_t i = 2; i < tab.size(); i++)
				tab[1] += " " + tab[i];
			tab[1].erase(0, 1);
			chan_dest->second.setTopic(tab[1]);
			chan_dest->second.send_message(*user, "TOPIC " + tab[0] + " :" + chan_dest->second.getTopic(), true);
		}
	}                     
}

void	Server::motd(std::vector<User>::iterator user, std::pair<bool, std::string> motd) {
	if (motd.first == false)
		return ;
	std::cout  << GR << "MOTD" << NC << std::endl;
	if (motd.second.empty() || motd.second.compare("localhost") == 0) 
		this->motd(user);
	else	
		user->send_error(to_string(ERRNOSUCHSERVER), motd.second + " :No such server");
}

void	Server::motd(std::vector<User>::iterator user) {
	user->send_message(to_string(RPL_MOTDSTART), "MOTD :- " + user->get_hostName() + " Message of the day -");
	user->send_message(to_string(RPL_MOTD), "MOTD :	 _       __  ______ __________ _    __ __________ ");
	user->send_message(to_string(RPL_MOTD), "MOTD :	| |     / / /  ___// ____/ __ \\ |  / // ____/ __ \\");
	user->send_message(to_string(RPL_MOTD), "MOTD :	| | /| / /  \\___ \\/ __/ / /_/ / | / // __/ / /_/ /");
	user->send_message(to_string(RPL_MOTD), "MOTD :	| |/ |/ /   ___/ / /___/ _, _/| |/ // /__ / _, _/");
	user->send_message(to_string(RPL_MOTD), "MOTD :	|__/|__/   /____/_____/_/ |_| |___//____ /_/ |_|");
	user->send_message(to_string(RPL_ENDOFMOTD), "MOTD :End of MOTD command");
}

void	Server::notice(std::vector<User>::iterator user, std::pair<bool, std::string> str) {
	if (str.first == false)
		return ;
	std::cout  << GR << "NOTICE" << NC << std::endl;
	std::vector<std::string> tab = split(str.second, ' ');
	std::string	msg;
    if (tab.size() < 2) { return ;}
    std::vector<User>::iterator user_dest = this->find_user(tab[0]);
    std::map<std::string, Channel>::iterator chan_dest = _channels.find(tab[0]);
	msg = tab[1];
	for (size_t i = 2; i < tab.size(); i++) {
		msg += " " + tab[i];
	}
	msg.erase(0, 1);
	if (user_dest != this->_users.end())
		user_dest->relay_message(*user, "NOTICE " + user_dest->get_nickName() + " :" + msg);
	else if (chan_dest != _channels.end() && chan_dest->second.users.find(user->get_fd()) != chan_dest->second.users.end()) 
			chan_dest->second.send_message(*user, "NOTICE " + tab[0] + " :" + msg, true);
}

void	Server::names(std::vector<User>::iterator user, std::pair<bool, std::string> str) {
	if (str.first == false)
		return ;
	std::cout  << GR << "NAMES" << NC << std::endl;
	std::vector<std::string> tab = split(str.second, ' ');
	if ((tab.size() == 2 && !tab[1].empty() && tab[1].compare("localhost") != 0) || tab.size() > 2) {
		user->send_error(to_string(ERRNOSUCHSERVER), tab[1] + " :No such server");
	} else if (tab.size() == 1 || (tab.size() == 2 && tab[1].compare("localhost") == 0)) {
		std::vector<std::string> channel = split(tab[0], ',');
		for (size_t i = 0; i < channel.size(); i++) {
			if (this->_channels.count(channel[i])) {
				std::map<int, User>::iterator users = this->_channels.find(channel[i])->second.users.begin();
				for (; users != this->_channels.find(channel[i])->second.users.end(); users++) {
					if (users->second.get_mode('i') == false || user->get_isOperator() == true)
						user->send_message(to_string(RPL_NAMREPLY), "=" + channel[i] + " :" + users->second.get_nickName());
				}
			}
		}
		user->send_other_error(to_string(RPL_ENDOFNAMES), "NAMES :End of NAMES list");
	} 
}

void	Server::invite(std::vector<User>::iterator user, std::pair<bool, std::string> str) {
	if (str.first == false)
		return ;
	std::cout  << GR << "INVITE" << NC << std::endl;
	std::map<std::string, Channel>::iterator chan_dest;
	std::vector<User>::iterator user_dest;
	std::vector<std::string> tab = split(str.second, ' ');
	if (tab.size() != 2) {
		user->send_error(to_string(ERRNEEDMOREPARAMS), "INVITE :Not enough parameters");
		return ;
	} else if ((chan_dest = this->_channels.find(tab[1])) == this->_channels.end()) {
		user->send_error(to_string(ERRNOSUCHCHANNEL), tab[1] + " :No such channel");
		return ;
	} else if (chan_dest->second.users.find(user->get_fd()) == chan_dest->second.users.end()) {
		user->send_error(to_string(ERRNOTONCHANNEL), tab[1] + " :You're not on that channel");
		return ;
	} else if ((user_dest = this->get_user(tab[0])) == this->_users.end()) {
		user->send_error(to_string(ERRNOSUCHNICK), tab[0] + " :No such nick");
		return ;
	} else if (chan_dest->second.users.find(user_dest->get_fd()) != chan_dest->second.users.end()) {
		user->send_error(to_string(ERRUSERONCHANNEL), tab[0] + " :is already on channel");
		return ;
	} else if (chan_dest->second.isOperator(user->get_fd()) == false) {
		user->send_error(to_string(ERRCHANOPRIVSNEED), tab[1] + " :You're not channel operator");
		return ;
	} else {
		user->send_message(to_string(RPL_INVITING), "INVITE " + tab[0] + " " + tab[1]);
		user_dest->relay_message(*user, "INVITE " + tab[0] + " " + tab[1]);
		this->join(user_dest, std::pair<bool, std::string>(true, tab[1]), true);
	}
}

void	Server::kick(std::vector<User>::iterator user, std::pair<bool, std::string> str) {
	if (str.first == false)
		return ;
	std::cout  << GR << "KICK" << NC << std::endl;
	std::map<std::string, Channel>::iterator chan_dest;
	std::vector<User>::iterator user_dest;
	std::vector<std::string> tab = split(str.second, ' ');
	std::string	msg;
	if (tab.size() >= 3)
		for (unsigned long i = 2; i < tab.size(); i++)
			msg += " " + tab[i];
	if (tab.size() < 3) {
		user->send_error(to_string(ERRNEEDMOREPARAMS), "KICK :Not enough parameters");
		return ;
	} else if ((chan_dest = this->_channels.find(tab[0])) == this->_channels.end()) {
		user->send_error(to_string(ERRNOSUCHCHANNEL), tab[0] + " :No such channel");
		return ;
	} else if (chan_dest->second.users.find(user->get_fd()) == chan_dest->second.users.end()
			|| chan_dest->second.users.find(this->get_user(tab[1])->get_fd()) == chan_dest->second.users.end()) {
		user->send_error(to_string(ERRUSERNOTINCHANNEL), " :User not in channel");
		return ;
	} 
	else if (tab[1] == user->get_nickName()) {
		user->send_error(to_string(ERRUSERSDONTMATCH), " : You can't kick yourself");
		return ;
	} else if (chan_dest->second.isOperator(user->get_fd()) == false) {
		user->send_error(to_string(ERRCHANOPRIVSNEED), " :You're not an operator");
		return ;
	} else {
		std::vector<User>::iterator	it = this->_users.begin();
		std::vector<User>::iterator	to_delete;
		while (it != this->_users.end()) {
			if (it->get_nickName().compare(tab[1]) == 0)
				to_delete = it;
			it++;
		}
		chan_dest->second.send_message(*user, "KICK " + tab[0] + " :" + tab[1] + msg, true);
		chan_dest->second.send_message(*to_delete, "PART " + tab[0] + " :" + msg, true);
		chan_dest->second.setKickStatus(to_delete->get_fd());
		chan_dest->second.users.erase(to_delete->get_fd());
	}
}

bool	Server::kill(std::vector<User>::iterator user, std::pair<bool, std::string> str) {
	if (str.first == false)
		return false;
	std::cout  << GR << "KILL" << NC << std::endl;
	std::vector<std::string> tab = split(str.second, ' ');
	std::string	msg;
	if (tab.size() < 2 || (tab[1].size() <= 1 && tab.size() == 2)) {
		user->send_error(to_string(ERRNEEDMOREPARAMS), "KILL :Not enough parameters");
	} else if (this->get_user(tab[0]) == this->_users.end()) {
		user->send_error(to_string(ERRNOSUCHNICK), tab[0] + " :No such nick");
	} else if (user->get_isOperator() == false) {
		user->send_error(to_string(ERRNOPRIVILEGES), user->get_nickName() + " :You're not an operator");
	} else {
		user->clear_buffer();
		for (unsigned long j = 1; j < tab.size(); j++)
			msg += " " + tab[j];
		std::map<std::string, Channel>::iterator it = _channels.begin();
		while (it != _channels.end()) {
			if (it->second.users.find(this->get_user(tab[0])->get_fd()) != it->second.users.end()) {
				std::cout << GRN << "KILL " << it->first << NC << std::endl;
				std::map<std::string, Channel>::iterator tmp = it;
				it++;
				this->part(this->get_user(tab[0]), std::make_pair(true, tmp->first));
			}
		}
		for (size_t i = 0; i < _pfds.size(); i++) {
			if (get_user(tab[0])->get_fd() == _pfds[i].fd) {
				get_user(tab[0])->send_message(to_string(RPL_KILLDONE), user->get_nickName() + " KILL " + tab[0] + " : " + msg);
				_bannedList.push_back(tab[0]);
				_users.erase(this->get_user(_pfds[i].fd));
				int tmp = _pfds[i].fd;
				_pfds.erase(_pfds.begin() + i);
				close(tmp);
				return true;
			}
		}
	}
	return false;
}

// METHODS

bool	Server::parse_packets(std::string packets, int fd) {
	std::vector<User>::iterator user = this->get_user(fd);

	this->nick(user, this->getInfo(user, "NICK", packets));
	this->user(user, this->getInfo(user, "USER", packets));
	this->ping(user, this->getInfo(user, "PING", packets));
	this->whois(user, this->getInfo(user, "WHOIS", packets));
	this->list(user, this->getInfo(user, "LIST", packets));
	this->oper(user, this->getInfo(user, "OPER", packets));
	this->privmsg(user, this->getInfo(user, "PRIVMSG", packets));
	this->join(user, this->getInfo(user, "JOIN", packets), false);
	this->mode(user, this->getInfo(user, "MODE", packets));
	this->part(user, this->getInfo(user, "PART", packets));
	this->topic(user, this->getInfo(user, "TOPIC", packets));
	this->motd(user, this->getInfo(user, "motd", packets));
	this->notice(user, this->getInfo(user, "NOTICE", packets));
	this->invite(user, this->getInfo(user, "INVITE", packets));
	this->kick(user, this->getInfo(user, "KICK", packets));
	if (this->kill(user, this->getInfo(user, "kill", packets)) == false)
		user->clear_buffer();
	if (user->get_isConnected() == false && user->get_nickName().size() != 0 && user->get_userName().size() != 0) {
		for (size_t i = 0; i < _bannedList.size(); i++) {
			if (user->get_nickName() == _bannedList[i])
			{
				user->send_message(to_string(ERRYOUREBANNEDCREEP), ":You're banned from this server"); 
				close(fd);
				for (size_t j = 0; j < _pfds.size(); j++) {
        			if (user->get_fd() == _pfds[j].fd)
						_pfds.erase(_pfds.begin() + j);
				}
				_users.erase(this->get_user(fd));
			}
		}
		user->set_isConnected(true);		
		user->send_message(to_string(RPL_WELCOME), user->get_nickName() + " :Welcome to the Internet Relay Network " + user->get_nickName() + "!"+ user->get_userName() +"@"+ user->get_hostName());
		user->print_user();
		this->motd(user);
	}
	// user->clear_buffer();
	return (this->die(user, this->getInfo(user, "die", packets)));
}

std::pair<bool, std::string> Server::getInfo(std::vector<User>::iterator user, std::string to_find, std::string buffer) {
	size_t begin = buffer.find(to_find);
	bool noise_before = false;
	if (begin != 0 && begin != std::string::npos) {
		if (buffer[begin - 1] != '\n')
			noise_before = true;
	}
	size_t end = buffer.find("\n", begin);
	if (begin == std::string::npos || end == std::string::npos || begin >= end)
		return (std::make_pair(false, ""));
	begin += to_find.size() + 1;
	end = begin;
	while (buffer[end] && buffer[end] != '\r' && buffer[end] != '\n')
		end++;
	std::string ret = buffer.substr(begin, end - begin);
	if (noise_before == true) {
		user->send_message(to_string(ERRUNKNOWNCOMMAND), " :Unknown command");
		return (std::make_pair(false, ""));
	}
	return std::make_pair(true, ret);
}

void	Server::server_loop() {
	int	num_events;
	bool stop = false;

	_pfds.push_back(pollfd());
	_pfds.back().events = POLLIN;
	_pfds.back().fd = _sockfd;
	while (!stop) {
		print_all();
		std::cout << CYN << "SERVER: waiting for connections..." << NC << std::endl;
		num_events = poll(&_pfds[0], _pfds.size(), -1);
		if (num_events == -1) {
			std::cout  << RED << "ERROR: POLL failed" << NC << std::endl;
		} else {
			if (_pfds[0].revents == POLLIN) {
				struct sockaddr_in  their_addr; // connector's address information
				socklen_t		   sin_size = sizeof(their_addr);
				int				 new_fd = accept(_pfds[0].fd, (struct sockaddr *)&their_addr, &sin_size);
				if (new_fd == -1) {
					std::cerr << RED << "ERROR: accept failed" << NC << std::endl;
				} else {
					char hostname[NI_MAXHOST];
					fcntl(new_fd, F_SETFL, O_NONBLOCK);
					if (getnameinfo((struct sockaddr *)&their_addr, sin_size, hostname, NI_MAXHOST, NULL, 0,  NI_NUMERICSERV) != 0)
						std::cerr << RED << "ERROR : getnameinfo() failed" << NC << std::endl;
					else {
					_pfds.push_back(pollfd());
					_pfds.back().events = POLLIN;
					_pfds.back().fd = new_fd;
					_users.push_back(User(new_fd, hostname));
					std::cout  << GRN << "SERVER: new connection from " << hostname << " on socket " << new_fd << NC << std::endl;
				}}
			} else {
				stop = this->receive();
			}
		}
	}
	while (_pfds.size() > 0) {
		close(_pfds.back().fd);
		_pfds.pop_back();
	}
}

bool	Server::receive() {
	bool		stop = false;
	for (size_t i = 1; i < _pfds.size(); i++) {
		if (_pfds[i].revents == POLLIN) {
			char packets[LEN_MAX_PACKETS];

			int size = recv(_pfds[i].fd, packets, LEN_MAX_PACKETS, MSG_WAITALL);
			this->get_user(_pfds[i].fd)->set_buffer(packets);
			std::string buffer = this->get_user(_pfds[i].fd)->get_buffer();
			if (size == -1)
				std::cerr << RED << "ERROR: recv() failed" << NC << std::endl;
			else if (size == 0) {
				std::cerr << RED << "ERROR: Client " << this->get_user(_pfds[i].fd)->get_nickName() << " disconnected" << NC << std::endl;
				_users.erase(this->get_user(_pfds[i].fd));
				int tmp = _pfds[i].fd;
				_pfds.erase(_pfds.begin() + i);
				close(tmp);
			}
			if (buffer.find("\n") != std::string::npos) {
				std::pair<bool, std::string> pass = this->getInfo(this->get_user(_pfds[i].fd), "PASS", std::string(buffer));
				if (pass.first == true  && pass.second.compare(_password) != 0){
					if (pass.second.size() == 0)
						this->get_user(_pfds[i].fd)->send_message(to_string(ERRNEEDMOREPARAMS), "PASS :Not enough parameters");
					std::cerr << RED << "ERROR: Wrong password" << NC << std::endl;
					close(_pfds[i].fd);
					_pfds.erase(_pfds.begin() + i);
					_users.erase(this->get_user(_pfds[i].fd));
				}
				else {
					std::cout  << GRN << "RECEIVE: " << buffer << NC << std::endl;
					stop = this->parse_packets(buffer, _pfds[i].fd);
				}
			}
			for (int j = 0; packets[j] != '\0'; j++)
				packets[j] = '\0';
		}
	}
	return (stop);
}

void	Server::print_all() {
	std::cout << "\x1B[2J\x1B[H";
	std::cout << MAG <<  "	 _       ____________________  __   _____ __________ _    __" << NC << std::endl;
	std::cout << MAG <<  "	| |     / /  _/_  __/ ____/ / / /  / ___// ____/ __ \\ |  / /" << NC << std::endl;
	std::cout << MAG <<  "	| | /| / // /  / / / /   / /_/ /   \\__ \\/ __/ / /_/ / | / / " << NC << std::endl;
	std::cout << MAG <<  "	| |/ |/ // /  / / / /___/ __  /   ___/ / /___/ _, _/| |/ /  " << NC << std::endl;
	std::cout << MAG <<  "	|__/|__/___/ /_/  \\____/_/ /_/   /____/_____/_/ |_| |___/   " << NC << std::endl;
                                                            
	std::cout << std::endl << std::endl;
	std::cout << YEL << "SERVER: " << _users.size() << " clients connected :" << NC << std::endl;
	for (size_t i = 0; i < _users.size(); i++)
		std::cout << "    - " << _users[i].get_nickName() << BLU <<  "	mode : " << _users[i].get_mode() << NC << std::endl;
	std::cout << std::endl << std::endl;
	std::cout << YEL << "SERVER: " << _channels.size() << " channels :" << NC << std::endl;
	std::map<std::string, Channel>::iterator it = _channels.begin();
	while (it != _channels.end()) {
		std::cout << "    - " << it->first << BLU << ":	mode : " << it->second.getChannelMode() << NC << std::endl;
		std::cout << "		users in channel: " << GRN << it->second.userIsOn() << NC << std::endl;
		it++;
	}
	std::cout << std::endl << std::endl;
}