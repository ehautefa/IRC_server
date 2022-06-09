/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlucie <hlucie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:54:48 by ehautefa          #+#    #+#             */
/*   Updated: 2022/06/09 11:25:05 by hlucie           ###   ########.fr       */
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


class Server
{
	private:
		int                         _port;
		std::string                 _password;
		std::vector<struct pollfd>  _pfds;
		std::vector<User>           _users;
		std::vector<Channel>        _channels;
		int                         _sockfd;

	public:
		Server(int port, std::string password);
		~Server();
		
		bool        set_sockfd(int sockfd);
		std::string get_password();
		int get_port();
		std::vector<struct pollfd> get_pfds();
		std::vector<User>::iterator	get_user(int fd);
		std::vector<User>::iterator	get_user(std::string nickname);
		
		void                        server_loop();
		std::string                 getInfo(std::string to_find, std::string buffer);
		void                        receive();
		void        parse_packets(char *packets, int size);
};


#endif