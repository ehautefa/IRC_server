#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define LEN_MAX_PACKETS 512

#include <vector>
#include <map>
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

class Channel
{
	private:

		std::string						_name;
		std::string						_topic;
		std::vector<std::string>		_modeChannel;

	public:
		typedef std::pair<std::string, User> modeUser;
		std::map<std::string, modeUser>	users;
		Channel();
		Channel(std::string name);
		~Channel();
		
		std::string			getTopic() const;
        std::string         getName() const;
		void				setTopic(std::string topic);
		void				send_message(User fromWho, std::string msg, bool toWho);
		void				addUser(User user);
};


#endif