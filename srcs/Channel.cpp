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

// void	Channel::setTopic(std::string topic)
// {
	
// }