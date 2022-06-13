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
		int		 _fd;
		std::string _nickName;
		std::string _userName;
		std::string _hostName;
		std::string _fullName;
		bool		_isConnected;
		bool		_isOperator;
		std::string	_mode;
        std::string _away;

	public:
		User(int fd, std::string hostname);
		// User(const User &src);
		// User &operator=(const User &rhs);
		~User(void);

		// GETTERS

		int		get_fd(void) const;
		bool	get_isConnected(void) const;
		bool	get_mode(char mode) const;
		std::string get_mode(void) const;
		std::string get_nickName(void) const;
		std::string get_hostName(void) const;
		std::string get_fullName(void) const;
		std::string get_userName(void) const;
		std::string	get_away(void) const;
		bool		get_isOperator(void) const;

		// SETTER	

		bool		set_mode(char mode);
		void		set_nickName(std::string nickName);
		void		set_userName(std::string userName);
		void		set_hostName(std::string hostName);
		void		set_fullName(std::string fullName);
		void		set_isConnected(bool isConnected);
		void		set_isOperator(bool isOperator);

		void		print_user(void); // for debug
		int 		send_message(std::string rpl, std::string to_send);
		int 		send_error(std::string rpl, std::string to_send);
		
};

#endif
