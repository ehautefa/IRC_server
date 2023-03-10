#include "../include/User.hpp"

# define WRONG_INPUT 2
# define UNKNOWN_CMD 3

User::User() : _fd(), _mdp(false), _cmd_found(false), _nickName(), _userName(), _hostName(), _fullName(), _isConnected(false), _isOperator(false), _mode(""), _away(), packets("") {}

User::User(int fd, std::string hostname) : _fd(fd), _mdp(false), _cmd_found(false), _hostName(hostname), _isConnected(false), _isOperator(false), _mode(""), _away(), packets("") {}

User::~User(void)
{
	return ;
}

// GETTERS

bool	User::get_mode(char mode) const { 
	return(_mode.find(mode) != std::string::npos); }

std::string User::get_mode(void) const { return(_mode); }

int		User::get_fd(void) const { return(_fd); }

std::string	User::get_nickName(void) const { return(_nickName); }

std::string	User::get_hostName(void) const { return(_hostName); }

std::string	User::get_fullName(void) const { return(_fullName); }

std::string	User::get_userName(void) const { return(_userName); }

std::string User::get_buffer(void) const { return(_buffer); }

std::string	User::get_away(void) const { return(_away); }

bool	User::get_mdp(void) const { return(_mdp); }

bool	User::get_cmd_found(void) const { return(_cmd_found); }

bool	User::get_isConnected(void) const { return(_isConnected); }

bool	User::get_isOperator(void) const { return(_isOperator); }

void	User::clear_buffer(void) { 
	if (!_buffer.empty())
		_buffer.clear(); 
}


// SETTER

void	User::set_mdp(bool mdp) { _mdp = mdp; }

void	User::set_cmd_found(bool cmd_found) { _cmd_found = cmd_found; }

void	User::set_buffer(std::string buffer) {
	if (_buffer.empty() )
		_buffer = buffer;
	else if (buffer.size() > 0)
		_buffer += buffer;
}

bool	User::set_mode(char mode) {
	if (this->_mode.find(mode) != std::string::npos)
		return (false);
	this->_mode += mode;
	return (true);
}

void	User::delete_mode(char mode) {
	if (this->_mode.find(mode) != std::string::npos)
		this->_mode.erase(this->_mode.find(mode));
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
	to_send = ":" + this->get_hostName() + " " + rpl + " " + to_send;
	std::cout  << BLU << "Sending: " << to_send << NC << std::endl;
	to_send += "\r\n";
	return (send(this->_fd, to_send.c_str(), to_send.size(), 0));
}

int User::send_error(std::string rpl, std::string to_send) {
	to_send = ":" + this->get_nickName() + "!" + this->get_userName() + "@" + this->get_hostName() + " " + rpl + " " + to_send;
	std::cout  << RED << "Sending: " << to_send << NC << std::endl;
	to_send += "\r\n";
	return (send(this->_fd, to_send.c_str(), to_send.size(), 0));
}

int User::send_other_error(std::string rpl, std::string to_send) {
	to_send = ":" + this->get_hostName() + " " + rpl + " * " + to_send;
	std::cout  << RED << "Sending: " << to_send << NC << std::endl;
	to_send += "\r\n";
	return (send(this->_fd, to_send.c_str(), to_send.size(), 0));
}

int	User::relay_message(User from, std::string to_relay) {
	to_relay = ":" + from.get_nickName() + "!" + from.get_userName() + "@" + from.get_hostName() + " " + to_relay;
	std::cout  << BLU << "Relaying from " << from.get_nickName() << " to " << this->get_nickName() << ":" << to_relay << NC << std::endl;
	to_relay += "\r\n";
	return (send(this->_fd, to_relay.c_str(), to_relay.size(), 0));
}