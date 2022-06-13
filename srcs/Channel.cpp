#include "../include/Channel.hpp"

Channel::Channel() : _name(), _topic(), _users(), _modeChannel() {}

Channel::Channel(std::string name) : _name(name), _topic(), _users(), _modeChannel() {}

Channel::~Channel(void) {}

std::string Channel::getTopic() const { return (_topic); }

std::string Channel::getName() const { return (_name); }

void	Channel::setTopic(std::string topic) { _topic = topic; }

void	Channel::send_message(std::string msg) {
	std::map<std::string, User>::iterator	it = _users.begin();
	for (; it != _users.end(); it++) {
		it->second.send_message("", msg);
	}
}
