#include "../include/User.hpp"

# define WRONG_INPUT 2
# define UNKNOWN_CMD 3

User::User(int fd, std::string hostname) : _fd(fd), _hostName(hostname), _isConnected(false), _isOperator(false), _mode(), _away()
{
	this->_nickName = "";
	this->_userName = "";
	this->_fullName = "";
}

User::~User(void)
{
	return ;
}

// GETTERS

bool	User::get_mode(char mode) const { return(_mode.find(mode) != std::string::npos); }

std::string User::get_mode(void) const { return(_mode); }

int		User::get_fd(void) const { return(_fd); }

std::string	User::get_nickName(void) const { return(_nickName); }

std::string	User::get_hostName(void) const { return(_hostName); }

std::string	User::get_fullName(void) const { return(_fullName); }

std::string	User::get_userName(void) const { return(_userName); }

std::string	User::get_away(void) const { return(_away); }

bool	User::get_isConnected(void) const { return(_isConnected); }

bool	User::get_isOperator(void) const { return(_isOperator); }

// SETTER

bool	User::set_mode(char mode) {
	if (this->_mode.find(mode) != std::string::npos)
		return (false);
	this->_mode += mode;
	return (true);
}

void	User::set_nickName(std::string nickName) {
	if (nickName.length() > 0)
		this->_nickName = nickName;
}

void	User::set_userName(std::string userName) {
	if (userName.length() > 0 && _userName.size() == 0)
		this->_userName = userName;
}

void	User::set_fullName(std::string fullName) {
	if (fullName.length() > 0 && _fullName.size() == 0)
		this->_fullName = fullName;
}

void	User::set_isConnected(bool isConnected) {
	this->_isConnected = isConnected;
}

void	User::set_isOperator(bool isOperator) {
	this->_isOperator = isOperator;
}

void	User::print_user(void) {
	std::cout << YEL << "NICK: " << this->_nickName << " USERNAME: " << this->_userName << " HOSTNAME: " << this->_hostName << " FULL: " << this->_fullName;
	if (this->_isConnected)
		std::cout << " CONNECTED" << NC << std::endl;
	else
		std::cout << " NOT CONNECTED" << NC << std::endl;
}

int	User::send_message(std::string rpl, std::string to_send) {
	to_send = ":" + this->get_hostName() + " " + rpl + " " + to_send + "\r\n";
	std::cout << BLU << "Sending: " << to_send << NC << std::endl;
	return (send(this->_fd, to_send.c_str(), to_send.size(), 0));
}

int User::send_error(std::string rpl, std::string to_send) {
	to_send = ":" + this->get_nickName() + "!" + this->get_userName() + "@" + this->get_hostName() + rpl + to_send + "\r\n";
	std::cout << RED << "Sending: " << to_send << NC << std::endl;
	return (send(this->_fd, to_send.c_str(), to_send.size(), 0));
}

int User::send_other_error(std::string rpl, std::string to_send) {
	to_send = ":" + this->get_hostName() + " " + rpl + " * " + to_send + "\r\n";
	std::cout << RED << "Sending: " << to_send << NC << std::endl;
	return (send(this->_fd, to_send.c_str(), to_send.size(), 0));
}
