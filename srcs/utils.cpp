/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 11:25:30 by judecuyp          #+#    #+#             */
/*   Updated: 2021/05/21 14:39:43 by judecuyp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config.hpp"
#include "../include/utils.hpp"

/*
** Cut a 
*/
void	cut_path(std::string &path, std::string &root)
{
	if (path == "/")
		path = root;
	else
	{
		if (path.size() > 0 && path[0] == '/')
			path = path.substr(1);
		if (path.size() > 0 && path[path.size() - 1] == '/')
			path = path.substr(0, path.size() - 1);
	}
}

std::string ft_itoa(int n, std::string zebi)
{
	// char			str[4];
	int				len;
	unsigned int	u_nbr;

	len = n < 0 || n == 0 ? 1 : 0;
	u_nbr = n < 0 ? -n : n;
	while (u_nbr > 0)
	{
		u_nbr /= 10;
		len++;
	}
	// if ((str = (char*)malloc(sizeof(char) * (len + 1))) == NULL)
	// 	return (NULL);
	// str[len] = '\0';
	u_nbr = n < 0 ? -n : n;
	if (n < 0)
		zebi[0] = '-';
	while (--len >= (n < 0 ? 1 : 0))
	{
		zebi[len] = (u_nbr % 10) | 0x30;
		u_nbr /= 10;
	}
	return (zebi);
}

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