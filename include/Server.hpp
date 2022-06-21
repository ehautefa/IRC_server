/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Server.hpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: ehautefa <ehautefa@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/03/21 11:54:48 by ehautefa		  #+#	#+#			 */
/*   Updated: 2022/06/10 14:22:45 by ehautefa		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define LEN_MAX_PACKETS 512

#include <map>
#include <vector>
#include <string>
#include <cerrno>
#include <cstring>
#include <string>
#include <fstream>
#include <utility>

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
		int						 		_port;
		std::string						_password;
		std::vector<struct pollfd>  	_pfds;
		std::vector<User>		   		_users;
		std::map<std::string, Channel>  _channels;
		int						 		_sockfd;

	public:
		Server(int port, std::string password);
		~Server();
		
		bool        				set_sockfd(int sockfd);
		std::string 				get_password() const;
		int 						get_port() const;
		std::vector<struct pollfd> 	get_pfds() const;
		std::vector<User>::iterator	get_user(int fd);
		std::vector<User>::iterator	get_user(std::string nickname);
		std::vector<User>::iterator	find_user(std::string str);

		void	infoChannel(std::pair<bool, std::string> channel);
		int		isInStr(char toFind, std::string channelName);
		int		findChannel(std::string name);

		void                        	server_loop();
		std::pair<bool, std::string>	getInfo(std::string to_find, std::string buffer);
		bool                        	receive();
		bool	       					parse_packets(std::string packets, int size);

		// COMMANDS

		void	join(std::vector<User>::iterator user, std::pair<bool, std::string> channel, bool invite);
		void	user(std::vector<User>::iterator user, std::pair<bool, std::string> username);  
		void    nick(std::vector<User>::iterator user, std::pair<bool, std::string> nickname);      
		void	ping(std::vector<User>::iterator user, std::pair<bool, std::string> server);
		void	whois(std::vector<User>::iterator user, std::pair<bool, std::string> who);
		void	list(std::vector<User>::iterator user, std::pair<bool, std::string> channel);
		bool	die(std::vector<User>::iterator user, std::pair<bool, std::string> reason);
		void	oper(std::vector<User>::iterator user, std::pair<bool, std::string> channel);
		void    privmsg(std::vector<User>::iterator user, std::pair<bool, std::string> str);
		void	mode(std::vector<User>::iterator user, std::pair<bool, std::string> str);
		void	part(std::vector<User>::iterator user, std::pair<bool, std::string> part);
		void	topic(std::vector<User>::iterator user, std::pair<bool, std::string> topic);
		void	motd(std::vector<User>::iterator user, std::pair<bool, std::string> motd);
		void	notice(std::vector<User>::iterator user, std::pair<bool, std::string> notice);
		void	names(std::vector<User>::iterator user, std::pair<bool, std::string> channel);
		void	invite(std::vector<User>::iterator user, std::pair<bool, std::string> invite);
		void	kick(std::vector<User>::iterator user, std::pair<bool, std::string> str);
		void	kill(std::vector<User>::iterator user, std::pair<bool, std::string> str, int fd);
		void	who(std::vector<User>::iterator user, std::pair<bool, std::string> who);
		void	motd(std::vector<User>::iterator user);
		
		void	print_all();
};


#endif