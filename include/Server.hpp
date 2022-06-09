/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehautefa <ehautefa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:54:48 by ehautefa          #+#    #+#             */
/*   Updated: 2022/06/09 16:28:21 by ehautefa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define LEN_MAX_PACKETS 512

#include <vector>
#include <string>
#include <cerrno>
#include <cstring>
#include <string>
#include <fstream>

#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "Commands.hpp"
#include "User.hpp"
#include "Channel.hpp"

std::vector<std::string> split(const std::string &chaine, char delimiteur);


class Server
{
	private:
		int                         	_port;
		std::string                		_password;
		std::vector<struct pollfd>  	_pfds;
		std::vector<User>           	_users;
		std::map<std::string, Channel>  _channels;
		int                         	_sockfd;

    public:
        Server(int port, std::string password);
        ~Server();
        
        bool        				set_sockfd(int sockfd);
        std::string 				get_password();
		int 						get_port();
        std::vector<struct pollfd> 	get_pfds();
        std::vector<User>::iterator	get_user(int fd);
        std::vector<User>::iterator	get_user(std::string nickname);
        
        void                        server_loop();
        std::pair<bool, std::string> getInfo(std::string to_find, std::string buffer);
        bool                        receive();
        bool	       				parse_packets(char *packets, int size);

        // COMMANDS

        
        void	user(std::vector<User>::iterator user, std::pair<bool, std::string> username);  
        void    nick(std::vector<User>::iterator user, std::pair<bool, std::string> nickname);      
        void	ping(std::vector<User>::iterator user, std::pair<bool, std::string> server);
        void	whois(std::vector<User>::iterator user, std::pair<bool, std::string> who);
		void	list(std::vector<User>::iterator user, std::pair<bool, std::string> channel);
		bool	die(std::vector<User>::iterator user, std::pair<bool, std::string> reason);
};


#endif