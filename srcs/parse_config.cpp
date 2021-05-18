/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 00:12:51 by judecuyp          #+#    #+#             */
/*   Updated: 2021/05/18 09:48:00 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config.hpp"
#include "../include/utils.hpp"

/*
** TODO :
** Check si les champs sont sensibles à la casse
*/

/*
** put the string contain in the first place in the reader (only if reader size == 1)
** else return error (for error gestion later)
*/
int		conf_get_str(std::string &dest, std::list<std::string> &reader)
{
	if (reader.size() == 1)
	{
		dest = reader.front();
		return (SUCCESS);
	}
	return (ERROR);

}

/*
** Same as conf_get_str but in list
*/
int		conf_get_list(std::list<std::string> &dest, std::list<std::string> &reader)
{
	std::list<std::string>::iterator it = reader.begin();

	while (it != reader.end())
	{
		dest.push_back(*it);
		++it;
	}
	return (SUCCESS);
}

/*
** same as conf_get_str but in numeric value
*/
int		conf_get_num(size_t &dest, std::list<std::string> &reader)
{
	size_t i = 0;
	std::string tmp;

	if (reader.size() == 1)
	{
		tmp = reader.front();
		while (tmp[i])
		{
			if (!isdigit(tmp[i]))
				return (ERROR);
			++i;
		}
		dest = (size_t)atol(tmp.c_str());
		return (SUCCESS);
	}
	return (ERROR);
}

/*
** find a end-bracket in a string
** return true if the line contain only a '}' (or '}' with whitespaces), false otherwise
*/
bool	find_end_bracket(std::list<std::string> &line)
{
	if (line.size() == 1 && line.front() == "}")
		return (true);
	return (false);
}

/*
** Find the name of the server field in .conf file
** and erase the first whitespaces
*/
bool		find_config_elem(std::list<std::string> &line, std::string const &name)
{
	if (line.size() > 1 && line.front() == name)
	{
		line.pop_front();
		return (true);
	}
	return (false);
}

/*
**
*/
int		parse_location(std::ifstream &fd, t_config *c, std::string &line)
{
	std::string				reader;
	t_loc					loc;
	std::list<std::string>	tmp;
	int						brackets = 1;

	tmp = split_in_list(line, " \t");
	tmp.pop_front();
	tmp.pop_back();
	loc.location_match = tmp.back();
	if (tmp.size() == 2)
		loc.optional_modifier = tmp.front();
	while (std::getline(fd, reader))
	{
		tmp = split_in_list(reader, " \t\n\r\v\f");
		if (find_end_bracket(tmp))
		{
			brackets -= 1;
			break;
		}
		else if (find_config_elem(tmp, "http_methods"))
			conf_get_str(loc.http_methods, tmp);
		else if (find_config_elem(tmp, "body_size_limit"))
			conf_get_num(loc.body_size, tmp);
		else if (find_config_elem(tmp, "directory_files_search"))
			conf_get_str(loc.directory_files_search, tmp);
		else if (find_config_elem(tmp, "directory_listing"))
			conf_get_str(loc.directory_listing, tmp);
		else if (find_config_elem(tmp, "default_file_directory_request"))
			conf_get_str(loc.default_file_directory_request, tmp);
		else if (find_config_elem(tmp, "upload_files_location"))
			conf_get_str(loc.upload_files_location, tmp);
	}
	if (brackets != 0)
		return (ERROR);
	c->locations.push_back(loc);
	return (SUCCESS);
}

/*
** True if we have a location, false otherwise
*/
int		find_location(std::string &line)
{
	std::list<std::string> tmp;

	tmp = split_in_list(line, " \t");
	if ((tmp.size() == 3 || tmp.size() == 4) && tmp.front() == "location" && tmp.back() == "{")
		return (true);
	return (false);

}

/*
** Parsinf of the server section
*/
int		parse_serv(std::ifstream &fd, std::list<t_config> &conf)
{
	std::list<std::string>	tmp;
	std::string				reader;
	int						open_brackets = 1;
	t_config				c;

	while (std::getline(fd, reader))
	{
		tmp = split_in_list(reader, " \t\n\r\v\f");
		if (find_end_bracket(tmp))
		{
			open_brackets -= 1;
			break;
		}
		else if (find_location(reader))
			parse_location(fd, &c, reader);
		else if (find_config_elem(tmp, "host"))
			conf_get_str(c.host, tmp);
		else if (find_config_elem(tmp, "port"))
			conf_get_list(c.port, tmp);
		else if (find_config_elem(tmp, "index"))
			conf_get_list(c.index, tmp);
		else if (find_config_elem(tmp, "body_size_limit"))
			conf_get_num(c.body_size, tmp);
	}
	if (open_brackets != 0)
		return (ERROR);
	conf.insert(conf.end(), c);
	if (!conf.empty())
		conf.front().default_server = true;
	return (SUCCESS);
}

bool	find_server(std::string &line)
{
	std::list<std::string>  spliter;
	size_t					size;

	spliter = split_in_list(line, " \t");
	size = spliter.size();
	if (size < 1 || size > 2)
		return (false);
	if (size == 2 && spliter.front() == "server" && spliter.back() == "{")
		return (true);
	return (false);
}

int		parse_conf(std::string path, std::list<t_config> &conf)
{
	std::ifstream	fd(path);
	std::string		reader;

	if (!fd)
	{
		fd.close();
		std::cout << "No such file or directory." << std::endl;
		return (ERROR);
	}
	while (std::getline(fd, reader))
	{
		if (find_server(reader))
			parse_serv(fd, conf);
	}
	fd.close();
	return (SUCCESS);
}
