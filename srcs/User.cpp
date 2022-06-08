#include "../include/User.hpp"

# define WRONG_INPUT 2
# define UNKNOWN_CMD 3

User::User(int fd, std::string hostname) : _fd(fd), _hostName(hostname)
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

int		User::get_fd(void) const
{
	return (this->_fd);
}

// SETTER

void	User::set_nickName(std::string nickName)
{
	if (nickName.length() > 0 && _nickName.size() == 0)
		this->_nickName = nickName;
}

void	User::set_userName(std::string userName)
{
	if (userName.length() > 0 && _userName.size() == 0)
		this->_userName = userName;
}

void	User::set_fullName(std::string fullName)
{
	if (fullName.length() > 0 && _fullName.size() == 0)
		this->_fullName = fullName;
}

void	User::print_user(void)
{
	std::cout << YEL << "NICK: " << this->_nickName << " USERNAME: " << this->_userName << " HOSTNAME: " << this->_hostName << " FULL: " << this->_fullName << NC << std::endl;
}