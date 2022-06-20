#include "../include/Channel.hpp"

Channel::Channel() : _name(), _topic(), _modeChannel(), _bannedList(), users() {}

Channel::Channel(std::string name) : _name(name), _topic(), _modeChannel(), _bannedList(), users() {}

Channel::~Channel(void) {}

std::string Channel::getTopic() const { return (_topic); }

std::string Channel::getName() const { return (_name); }

std::string	Channel::getMode() const { return (_modeChannel); }

bool	Channel::getKickStatus(std::string name) const {

	for (int i = 0; i < _bannedList[i]; i++) {
		if (_bannedList.find(name) != std::string::npos) {
			return (true);
		}
	}
	return (false);
}

void	Channel::setTopic(std::string topic) { _topic = topic; }

void	Channel::setKickStatus(std::string name) { 
	this->_bannedList += " " + name;
}

std::string	Channel::userIsOn(void){

	std::string ret = "";
	std::map<int, User>::iterator	it = users.begin();
	for (; it != users.end(); it++) {
		if (this->isOperator(it->first) == true)
			ret += "@" + it->second.get_nickName() + " ";
		else if (this->isVoice((it->first) == true))
			ret += "+v" + it->second.get_nickName() + " ";
		else 
			ret += it->second.get_nickName() + " ";
	}
	return (ret);
}

bool	Channel::isOperator(int fd) const {
	if (this->_users_modes.find(fd)->second == 'o'
	|| this->_users_modes.find(fd)->second == 'O') 
		return (true);
	return false;
}

bool	Channel::isCreator(int fd) const {
	if (this->_users_modes.find(fd)->second == 'O')
		return (true);
	return false;
}

bool	Channel::isVoice(int fd) const {
	if (this->_users_modes.find(fd)->second == 'o'
		|| this->_users_modes.find(fd)->second == 'O'
		|| this->_users_modes.find(fd)->second == 'v')
		return (true);
	return false;
}

void	Channel::set_userMode(int fd, char mode) {
	this->_users_modes[fd] = mode;
}

void	Channel::delete_userMode(int fd, char mode) {
	if (this->_users_modes[fd] == 'o' && mode == 'o')
		this->_users_modes[fd] = 'v';
	else if (this->_users_modes[fd] == 'v' && mode == 'v')
		this->_users_modes[fd] = ' ';
}

void	Channel::setChannelMode(char c) {
	if (_modeChannel.find(c) == std::string::npos)
		_modeChannel += c;
}

void	Channel::deleteChannelMode(char c) {
	if (_modeChannel.find(c) != std::string::npos)
		_modeChannel.erase(_modeChannel.find(c), 1);
}

std::string		Channel::getCreator() const {
	std::map<int, User>::const_iterator	it = users.begin();
	for (; it != users.end(); it++) {
		if (this->isCreator(it->second.get_fd()))
			return (it->second.get_nickName());
	}
	return ("");
}



void	Channel::addUser(User user, char mode) {
	std::cout  << YEL << user.get_fd() << NC << std::endl;
	this->users[user.get_fd()] = user;
	this->_users_modes[user.get_fd()] = mode;
}

std::string	Channel::getChannelMode() const { return (_modeChannel); }
		
char		Channel::getUserMode(int fd) const { return (this->_users_modes.find(fd)->second); }

bool	Channel::getChannelMode(char c) const {
	if (_modeChannel.find(c) != std::string::npos)
		return (true);
	return (false);
}

void	Channel::send_message(User fromWho, std::string msg, bool toWho) {
	// toWho false : tout le monde sauf soi
	// toWho true : tout le monde inclu soi
	std::map<int, User>::iterator	it = users.begin();
	for (; it != users.end(); it++) {
		if (it->first != fromWho.get_fd() || toWho != false)
			it->second.relay_message(fromWho, msg);
	}
}