/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehautefa <ehautefa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 12:38:31 by hlucie            #+#    #+#             */
/*   Updated: 2022/06/08 13:19:19 by ehautefa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include "Colors.hpp"

class User
{

    private:
        int         _fd;
        std::string _nickName;
        std::string _userName;
        std::string _hostName;
        std::string _fullName;

    public:
        User(int fd, std::string hostname);
        // User(const User &src);
        // User &operator=(const User &rhs);
        ~User(void);

        // GETTERS

        int         get_fd(void) const;

        // SETTER

        void        set_nickName(std::string nickName);
        void        set_userName(std::string userName);
        void        set_hostName(std::string hostName);
        void        set_fullName(std::string fullName);

        void        print_user(void); // for debug
        
};

#endif
