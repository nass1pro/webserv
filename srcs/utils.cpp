/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 11:25:30 by judecuyp          #+#    #+#             */
/*   Updated: 2021/04/21 11:33:06 by judecuyp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config.hpp"
#include "../include/utils.hpp"

/*
** Split a string into a list of strings on the charset
** Charset is individuals characters not a word
** (maybe modify for accept word or invidividual ?)
** Split return the filled list<string>
*/
std::list<std::string> split_in_list(std::string str, std::string charset)
{
	std::list<std::string>	ret;
	std::string				cut;
	size_t					pos;
	size_t					start;

	start = str.find_first_not_of(charset);
	pos = str.find_first_of(charset, start);
	while (pos != std::string::npos)
	{
		
		cut = str.substr(start, pos - start);
		ret.push_back(cut);
		while (charset.find(str[pos]) != std::string::npos/* && pos < str.size()*/)
			++pos;
		start = pos;
		pos = str.find_first_of(charset, start);
	}
	if (start < str.size())
	{
		cut = str.substr(start, pos - start);
		ret.push_back(cut);
	}
	return (ret);
}

/*
** True if c is in charset otherwise return false
*/
bool	is_in_charset(char c, std::string charset)
{
	size_t i;
	size_t len;

	i = 0;
	len = charset.size();
	while (i < len)
	{
		if (c == charset[i])
			return (true);
		++i;
	}
	return (false);
}