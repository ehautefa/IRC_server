#include "../include/Channel.hpp"

Channel::Channel() : _name(), _topic(), _modeChannel(), users() {}

Channel::Channel(std::string name) : _name(name), _topic(), _modeChannel(), users() {}

Channel::~Channel(void) {}

std::string Channel::getTopic() const { return (_topic); }

std::string Channel::getName() const { return (_name); }

std::string	Channel::getMode() const { return (_modeChannel); }

void	Channel::setTopic(std::string topic) { _topic = topic; }

void	Channel::setMode(std::string mode) {
	std::cout << mode << std::endl;
	_modeChannel += mode;}

std::string	Channel::userIsOn(void){

	std::string ret = "";
	std::map<int, User>::iterator	it = users.begin();
	for (; it != users.end(); it++) {
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