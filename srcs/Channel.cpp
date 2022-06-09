#include "../include/Channel.hpp"

Channel::Channel()
{
	return ;
}

Channel::Channel(std::string name)
{
	this->_name = name;
	return ;
}

Channel::~Channel(void)
{
	return ;
}

const std::string Channel::getTopic()
{
	return (this->_topic);
}

void	Channel::setTopic(std::string topic)
{
	(void)topic;
}

// const std::string	Channel::getName()
// {
// 	return (this->_name);
// }