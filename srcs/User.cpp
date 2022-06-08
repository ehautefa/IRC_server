#include "../include/User.hpp"

# define WRONG_INPUT 2
# define UNKNOWN_CMD 3

User::User(std::string nick, std::string name, std::string fullname, std::string host)
{
	(void)nick;
	(void)name;
	(void)fullname;
	(void)host;
	return ;
}

User::~User(void)
{
	return ;
}

int	createUser(std::string buf) {

	std::string			cmd;
	const std::string	lst_cmd[2] = {"/connect", "/msg"};
	int					i;

	i = 0;
	cmd = buf;
	std::cout << GRN << "SERVER receive: " << cmd << NC << std::endl;
	while (cmd[i] && cmd[i] == ' ')
		i++;
	if (cmd[i] && cmd[i] != '/')
	{
		std::cout << RED << "Wrong format" << NC << std::endl;
		return (WRONG_INPUT);
	}
	i = 0;
	while (i < 2)
	{
		if (lst_cmd[i] == cmd)
		{
			std::cout << "Index : " << i << " cmd : " << cmd << std::endl;
			return 1;
		}
		else
		{
			std::cout << "Unknown command: " << cmd << std::endl;
			return (UNKNOWN_CMD);
		}
		i++;
	}
	return (0);
}
