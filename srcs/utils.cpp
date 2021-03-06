/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stuntman <stuntman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 11:25:30 by judecuyp          #+#    #+#             */
/*   Updated: 2021/07/12 17:32:02 by stuntman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config.hpp"
#include "../include/utils.hpp"

/*
** Check if a file exist
** compare a filename with a string list of filenames in a directory
*/
bool	is_file_exist(std::string &url, std::string &no_file_path, std::list<std::string> &files_list)
{
	std::string							to_find = url.substr(no_file_path.size());
	std::list<std::string>::iterator	it = files_list.begin();

	while (it != files_list.end())
	{
		if (*it == to_find)
			return (true);
		++it;
	}
	return (false);
}

/*
** Take an url, cut the file, check if the directory path exist and list the files in a list
*/
bool	is_exist(std::string &url)
{
	std::list<std::string>	files_list;
	std::string				no_file_path = url;
	struct dirent			*file;
	DIR						*directory;


	if (!no_file_path.empty() && no_file_path[no_file_path.size() - 1] != '/')
		no_file_path.erase(no_file_path.find_last_of("/") + 1);
	if ((directory = opendir(no_file_path.c_str())) == 0)
		return (false);
	while ((file = readdir(directory)) != 0)
	{
		if (file->d_type != DT_DIR)
			files_list.push_back(file->d_name);
	}
	closedir(directory);
	return (is_file_exist(url, no_file_path, files_list));
}

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

std::list<std::string> split_body(std::string &str)
{
	std::list<std::string>		line_of_body;
	std::string					line;
	const size_t				size_body = str.size();

	for (size_t j = 0; j < size_body; j++)
	{
		if (j == 0 && str[j] == '\n')
			continue ;
		else if (str[j] == '\n')
		{
			line_of_body.push_back(line);
			line.clear();
		}
		else
			line += str[j];
	}
	return (line_of_body);
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
		if (pos != std::string::npos)
			pos = str.find_first_of(charset, start);
		else
			pos = str.find_first_of(charset, str.size() - 1);
		
	}
	if (start < str.size())
	{
		if (pos == std::string::npos)
			pos = str.size();
		cut = str.substr(start, pos - start);
		ret.push_back(cut);
	}
	return (ret);
}

bool is_white_space(char c)
{
	if (c == '\n' || c == ' ' || c == '\r' || c == '\v' || c == '\f' || c == '\t')
		return true;
	return false;
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

bool	directory_path_exist(std::string &path, std::string &root)
{
	DIR	*directory;
	std::string path_test = root;
	if (!path.empty() && path[0] == '/')
		path_test += path.substr(1, path.size() - 1);
	else
		path_test += path;
	if (path_test[path_test.size() - 1] != '/')
		path_test += "/";
	if ((directory = opendir(path_test.c_str())) == NULL)
		return (false);
	closedir(directory);
	return (true);
}