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
		std::string			getCreator() const;
		std::string			getChannelMode() const;
		bool				getChannelMode(char c) const;
		void				setChannelMode(char c);
		void				deleteChannelMode(char c);
		bool				isOperator(int fd) const;
		bool				isCreator(int fd) const;
		bool				isVoice(int fd) const;
		void				setTopic(std::string topic);
		void				addUser(User user, char mode);
		char				getUserMode(int fd) const;
		void				set_userMode(int fd, char mode);
		void				delete_userMode(int fd, char mode);
		void				send_message(User fromWho, std::string msg, bool toWho);
		std::string			userIsOn(void);
};


#endif