#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define LEN_MAX_PACKETS 512

#include <vector>
#include <string>
# include <cerrno>
# include <cstring>
# include <string>
# include <fstream>

#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "Commands.hpp"
#include "User.hpp"
#include "Commands.hpp"

class Channel
{
	private:

		std::string                 _name;
		std::string                 _creator;
		std::vector<User>           _users;
		std::vector<std::string>    _modeChannel;
		std::vector<std::string>    _modeUsers;
		std::vector<std::string>    _operator;

	public:

		Channel(std::string name);
		~Channel();
		
		std::vector<User>::iterator	get_user(int fd);
		std::vector<User>::iterator	get_user(std::string nickname);
};


#endif