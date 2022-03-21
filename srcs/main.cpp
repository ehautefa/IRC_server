#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"
#define GR "\e[0;90m"
#define BL "\e[0;30m"

#include <iostream>
#include <string>
#include <cctype>
#include <exception>
#define PASSWORD "12345"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << RED << "Attempt format is \"./ircserv <port> <password>\"" << NC << std::endl;
		return (1);
	}
	std::string	port = argv[1];
	std::string	passWord = argv[2];
	for (unsigned int i = 0; port[i] != '\0'; i++)
	{
		if (!std::isdigit(port[i]) && port[i] != '.')
		{
			std::cerr << RED << "Error : wrong format input" << NC << std::endl;
			return (1);
		}
	}
	if (passWord != PASSWORD)
	{
		std::cerr << RED << "Error : wrong password input" << NC << std::endl;
		return (1);
	}
	return (0);
}