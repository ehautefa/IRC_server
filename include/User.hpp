/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   User.hpp										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: ehautefa <ehautefa@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/03/30 12:38:31 by hlucie			#+#	#+#			 */
/*   Updated: 2022/06/10 14:24:12 by ehautefa		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include "Commands.hpp"
#include <iostream>
#include <netdb.h>
#include "Colors.hpp"

class User
{

	private:
		int		 	_fd;
		bool		_mdp;
		bool		_cmd_found;
		std::string _nickName;
		std::string _userName;
		std::string _hostName;
		std::string _fullName;
		bool		_isConnected;
		bool		_isOperator;
		std::string	_mode;
        std::string _away;
		std::string _buffer;

	public:
		User();
		User(int fd, std::string hostname);
		~User(void);

		std::string packets;

		// GETTERS

		int			get_fd(void) const;
		bool		get_isConnected(void) const;
		bool		get_mode(char mode) const;
		bool		get_mdp(void) const;
		bool		get_cmd_found(void) const;
		std::string get_mode(void) const;
		std::string get_nickName(void) const;
		std::string get_hostName(void) const;
		std::string get_fullName(void) const;
		std::string get_userName(void) const;
		std::string	get_away(void) const;
		bool		get_isOperator(void) const;
		std::string	get_buffer(void) const;
		void		clear_buffer(void);

		// SETTER	

		bool		set_mode(char mode);
		void		set_mdp(bool mdp);
		void		set_cmd_found(bool cmd_found);
		void		set_nickName(std::string nickName);
		void		set_userName(std::string userName);
		void		set_hostName(std::string hostName);
		void		set_fullName(std::string fullName);
		void		set_isConnected(bool isConnected);
		void		set_isOperator(bool isOperator);
		void		set_buffer(std::string buffer);

		void		delete_mode(char mode);
		void		print_user(void); // for debug
		int 		send_message(std::string rpl, std::string to_send);
		int 		send_error(std::string rpl, std::string to_send);
		int			relay_message(User from, std::string to_relay);
		int 		send_other_error(std::string rpl, std::string to_send);
		int			sendMessageToAll(User fromWho, std::string to_send, bool toWho);	
};

#endif
