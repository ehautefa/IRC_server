/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehautefa <ehautefa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 17:48:05 by ehautefa          #+#    #+#             */
/*   Updated: 2022/06/09 09:25:06 by ehautefa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <iostream>
#include <sstream>


std::vector<std::string> split(const std::string &chaine, char delimiteur)
{
	std::vector<std::string> elements;
	std::stringstream ss(chaine);
	std::string sousChaine;
	while (std::getline(ss, sousChaine, delimiteur))
	{
		elements.push_back(sousChaine);
 	}
	 return elements;
}