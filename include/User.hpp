/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlucie <hlucie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 12:38:31 by hlucie            #+#    #+#             */
/*   Updated: 2022/03/30 12:56:42 by hlucie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <string>
#include <iostream>
#include "Colors.hpp"

class	User {

private : 
	std::string	_nickName;
	std::string	_userName;
	std::string	_hostName;
	std::string	_fullName;

public :
	User(std::string nick, std::string name, std::string fullname, std::string host);
	User(const User &src);
	User &operator=(const User &rhs);
	~User(void);
};

int createUser(std::string buf);

#endif