#include "../include/Channel.hpp"

Channel::Channel() : _name(), _topic(), _modeChannel(), users() {}

Channel::Channel(std::string name) : _name(name), _topic(), _modeChannel(), users() {}

Channel::~Channel(void) {}

std::string Channel::getTopic() const { return (_topic); }

std::string Channel::getName() const { return (_name); }

void	Channel::setTopic(std::string topic) { _topic = topic; }

void	Channel::send_message(User fromWho, std::string msg, bool toWho) {
	// toWho false : tout le monde sauf soi
	// toWho true : tout le monde inclu soi
	std::map<std::string, User>::iterator	it = users.begin();
	for (; it != users.end(); it++) {
		if (it->first.compare(fromWho.get_nickName()) != 0 || toWho != false)
			it->second.relay_message(fromWho, msg);
	}
}