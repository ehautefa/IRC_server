/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlucie <hlucie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 11:55:04 by hlucie            #+#    #+#             */
/*   Updated: 2022/03/23 12:26:46 by hlucie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

#include <string>
#include <iostream>
#include "Colors.hpp"

class   Command {

private :
    std::string _cmd;
public :
    Command(char buf[1024]);
    ~Command(void);
    int checkArg(char buf[1024]);

};

#endif