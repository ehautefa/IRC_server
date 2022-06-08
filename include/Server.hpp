/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehautefa <ehautefa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:54:48 by ehautefa          #+#    #+#             */
/*   Updated: 2022/06/08 13:41:08 by ehautefa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define LEN_MAX_PACKETS 512

#include <vector>
#include <string>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "User.hpp"


class Server
{
    private:
        int                         _port;
        std::string                 _password;
        std::vector<struct pollfd>  _pfds;
        std::vector<User>           _users;
        int                         _sockfd;

    public:
        Server(int port, std::string password);
        ~Server();
        
        bool        set_sockfd(int sockfd);
        std::string get_password();
        int get_port();
        std::vector<struct pollfd> get_pfds();
        std::vector<User>::iterator	get_user(int fd);
        
        void                        server_loop();
        std::string                 getInfo(std::string to_find, std::string buffer);
        void                        receive();
        void        parse_packets(char *packets, int size);
};


#endif