#include "../include/Channel.hpp"

Channel::Channel() : _name(), _topic(), _modeChannel(), _bannedList(), users() {}

Channel::Channel(std::string name) : _name(name), _topic(), _modeChannel(), _bannedList(), users() {}

Channel::~Channel(void) {}

std::string Channel::getTopic() const { return (_topic); }

std::string Channel::getName() const { return (_name); }

std::string	Channel::getMode() const { return (_modeChannel); }

bool	Channel::getKickStatus(std::string name) const {

	for (int i = 0; i < _bannedList[i]; i++) {
		if (_bannedList.find(name) != std::string::npos)
		{
			std::cout << " TRUE : " << name << std::endl;
			return (true);
		}
	}
	std::cout << " FALSE : " << name << std::endl;
	return (false);
}

void	Channel::setTopic(std::string topic) { _topic = topic; }

void	Channel::setKickStatus(std::string name) { 

	std::cout << "HERE : " << name << std::endl;
	this->_bannedList += " " + name;
	std::cout << "LIST : " << this->_bannedList << std::endl;
}

void	Channel::setMode(std::string mode) {
	std::cout << mode << std::endl;
	_modeChannel += mode;}

std::string	Channel::userIsOn(void){

	std::string ret = "";
	std::map<int, User>::iterator	it = users.begin();
	for (; it != users.end(); it++) {
		// if (it->second.get_mode('o') == true || it->second.get_mode('O') == true)
		// 	ret += "@" + it->second.get_nickName() + " ";
		ret += it->second.get_nickName() + " ";
	}
	return (ret);
}

bool	Channel::isOperator(int fd) {
	if (this->_users_modes.find(fd)->second == 'o'
	|| this->_users_modes.find(fd)->second == 'O') 
		return (true);
	return false;
}

bool	Channel::isCreator(int fd) {
	if (this->_users_modes.find(fd)->second == 'O')
		return (true);
	return false;
}

bool	Channel::isVoice(int fd) {
	if (this->_users_modes.find(fd)->second == 'o'
		|| this->_users_modes.find(fd)->second == 'O'
		|| this->_users_modes.find(fd)->second == 'v')
		return (true);
	return false;
}

void	Channel::set_userMode(int fd, char mode) {
	this->_users_modes[fd] = mode;
}

void	Channel::addUser(User user, char mode) {
	std::cout  << YEL << user.get_fd() << NC << std::endl;
	this->users[user.get_fd()] = user;
	this->_users_modes[user.get_fd()] = mode;
}

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