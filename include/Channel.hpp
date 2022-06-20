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

		std::string				_name;
		std::string				_topic;
		std::string				_modeChannel;
		std::map<int, char>		_users_modes;

	public:
		std::map<int, User>		users;
		Channel();
		Channel(std::string name);
		~Channel();

		std::string			getTopic() const;
        std::string         getName() const;
		std::string			getChannelMode() const;
		bool				getChannelMode(char c) const;
		bool				isOperator(int fd);
		bool				isCreator(int fd);
		bool				isVoice(int fd);
		void				set_userMode(int fd, char mode);
		void				setTopic(std::string topic);
		void				send_message(User fromWho, std::string msg, bool toWho);
		void				addUser(User user, char mode);
		std::string			userIsOn(void);
};


#endif