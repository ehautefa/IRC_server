/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehautefa <ehautefa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 12:38:31 by hlucie            #+#    #+#             */
/*   Updated: 2022/06/09 13:35:38 by ehautefa         ###   ########.fr       */
/*                                                                            */
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
        int         _fd;
        std::string _nickName;
        std::string _userName;
        std::string _hostName;
        std::string _fullName;
        bool        _isConnected;
        bool        _isOperator;

    public:
        User(int fd, std::string hostname);
        // User(const User &src);
        // User &operator=(const User &rhs);
        ~User(void);

        // GETTERS

        int         get_fd(void) const;
        bool        get_isConnected(void) const;
        std::string get_nickName(void) const;
        std::string get_hostName(void) const;
        std::string get_fullName(void) const;
        std::string get_userName(void) const;
        bool        get_isOperator(void) const;

        // SETTER

        void        set_nickName(std::string nickName);
        void        set_userName(std::string userName);
        void        set_hostName(std::string hostName);
        void        set_fullName(std::string fullName);
        void        set_isConnected(bool isConnected);
        void        set_isOperator(bool isOperator);

        void        print_user(void); // for debug
        int 	    send_message(std::string rpl, std::string to_send);
        
};

#endif
