#include "../include/Command.hpp"

Command::Command(char buf[1024])
{
	if (checkArg(buf))
	{
		std::cout << RED << "Wrong command" << NC << std::endl;
		return ;
	}
}

int	Command::checkArg(char buf[1024]) {

	std::string	cmd;

	cmd = buf;
	std::cout << "HEY " << cmd << std::endl;
	return (0);
}