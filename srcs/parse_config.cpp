/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 00:12:51 by judecuyp          #+#    #+#             */
/*   Updated: 2021/07/05 11:25:13 by judecuyp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config.hpp"
#include "../include/utils.hpp"

// /*
// ** put the string contain in the first place in the reader (only if reader size == 1)
// ** else return error (for error gestion later)
// */
// int		conf_get_str(std::string &dest, std::list<std::string> &reader)
// {
// 	if (reader.size() == 1)
// 	{
// 		dest = reader.front();
// 		return (SUCCESS);
// 	}
// 	return (ERROR);

// }

// /*
// ** Same as conf_get_str but in list
// */
// int		conf_get_list(std::list<std::string> &dest, std::list<std::string> &reader)
// {
// 	std::list<std::string>::iterator it = reader.begin();

// 	while (it != reader.end())
// 	{
// 		dest.push_back(*it);
// 		++it;
// 	}
// 	return (SUCCESS);
// }

// /*
// ** same as conf_get_str but in numeric value
// */
// int		conf_get_num(size_t &dest, std::list<std::string> &reader)
// {
// 	size_t i = 0;
// 	std::string tmp;

// 	if (reader.size() == 1)
// 	{
// 		tmp = reader.front();
// 		while (tmp[i])
// 		{
// 			if (!isdigit(tmp[i]))
// 				return (ERROR);
// 			++i;
// 		}
// 		dest = (size_t)atol(tmp.c_str());
// 		return (SUCCESS);
// 	}
// 	return (ERROR);
// }

// /*
// ** find a end-bracket in a string
// ** return true if the line contain only a '}' (or '}' with whitespaces), false otherwise
// */
// bool	find_end_bracket(std::list<std::string> &line)
// {
// 	if (line.size() == 1 && line.front() == "}")
// 		return (true);
// 	return (false);
// }

// /*
// ** Find the name of the server field in .conf file
// ** and erase the first whitespaces
// */
// bool		find_config_elem(std::list<std::string> &line, std::string const &name)
// {
// 	if (line.size() > 1 && line.front() == name)
// 	{
// 		line.pop_front();
// 		return (true);
// 	}
// 	return (false);
// }

// /*
// ** parsing cgi
// */
// int		parse_cgi(t_loc &l, std::list<std::string> line)
// {
// 	l.cgi.active = true;
// 	if (find_config_elem(line, "AUTH_TYPE"))
// 		conf_get_str(l.cgi.AUTH_TYPE, line);
// 	else if (find_config_elem(line, "CONTENT_LENGTH"))
// 		conf_get_str(l.cgi.CONTENT_LENGTH, line);
// 	else if (find_config_elem(line, "CONTENT_TYPE"))
// 		conf_get_str(l.cgi.CONTENT_TYPE, line);
// 	else if (find_config_elem(line, "GATEWAY_INTERFACE"))
// 		conf_get_str(l.cgi.GATEWAY_INTERFACE, line);
// 	else if (find_config_elem(line, "PATH_INFO"))
// 		conf_get_str(l.cgi.PATH_INFO, line);
// 	else if (find_config_elem(line, "PATH_TRANSLATED"))
// 		conf_get_str(l.cgi.PATH_TRANSLATED, line);
// 	else if (find_config_elem(line, "QUERY_STRING"))
// 		conf_get_str(l.cgi.QUERY_STRING, line);
// 	else if (find_config_elem(line, "REMOTE_ADDR"))
// 		conf_get_str(l.cgi.REMOTE_ADDR, line);
// 	else if (find_config_elem(line, "REMOTE_IDENT"))
// 		conf_get_str(l.cgi.REMOTE_IDENT, line);
// 	else if (find_config_elem(line, "REMOTE_USER"))
// 		conf_get_str(l.cgi.REMOTE_USER, line);
// 	else if (find_config_elem(line, "REQUEST_METHOD"))
// 		conf_get_str(l.cgi.REQUEST_METHOD, line);
// 	else if (find_config_elem(line, "REQUEST_URI"))
// 		conf_get_str(l.cgi.REQUEST_URI, line);
// 	else if (find_config_elem(line, "SCRIPT_NAME"))
// 		conf_get_str(l.cgi.SCRIPT_NAME, line);
// 	else if (find_config_elem(line, "SERVER_NAME"))
// 		conf_get_str(l.cgi.SERVER_NAME, line);
// 	else if (find_config_elem(line, "SERVER_PORT"))
// 		conf_get_str(l.cgi.SERVER_PORT, line);
// 	else if (find_config_elem(line, "SERVER_PROTOCOL"))
// 		conf_get_str(l.cgi.SERVER_PROTOCOL, line);
// 	else if (find_config_elem(line, "SERVER_SOFTWARE"))
// 		conf_get_str(l.cgi.SERVER_SOFTWARE, line);
// 	return (SUCCESS);
// }

// /*
// ** Init location values
// */
// void	init_location(t_config &conf, t_loc &loc)
// {
// 	loc.body_size_limit = conf.body_size_limit;
// }

// /*
// ** parse the location in config file
// */
// int		parse_location(std::ifstream &fd, t_config &c, std::string &line)
// {
// 	std::string				reader;
// 	t_loc					loc;
// 	std::list<std::string>	tmp;
// 	int						brackets = 1;

// 	init_location(c, loc);
// 	tmp = split_in_list(line, " \t");
// 	tmp.pop_front();
// 	tmp.pop_back();
// 	loc.location_match = tmp.back();
// 	if (tmp.size() == 2)
// 		loc.optional_modifier = tmp.front();
// 	while (std::getline(fd, reader))
// 	{
// 		tmp = split_in_list(reader, " \t\n\r\v\f");
// 		if (find_end_bracket(tmp))
// 		{
// 			brackets -= 1;
// 			break;
// 		}
// 		else if (find_config_elem(tmp, "http_methods"))
// 			conf_get_list(loc.http_methods, tmp);
// 		else if (find_config_elem(tmp, "index"))
// 			conf_get_list(loc.index, tmp);
// 		else if (find_config_elem(tmp, "body_size_limit"))
// 			conf_get_num(loc.body_size_limit, tmp);
// 		else if (find_config_elem(tmp, "directory_files_search"))
// 			conf_get_str(loc.directory_files_search, tmp);
// 		else if (find_config_elem(tmp, "directory_listing"))
// 			conf_get_str(loc.directory_listing, tmp);
// 		else if (find_config_elem(tmp, "default_file_directory_request"))
// 			conf_get_str(loc.default_file_directory_request, tmp);
// 		else if (find_config_elem(tmp, "upload_files_location"))
// 			conf_get_str(loc.upload_files_location, tmp);
// 		else if (find_config_elem(tmp, "fastcgi_param"))
// 			parse_cgi(loc, tmp);
// 	}
// 	if (brackets != 0)
// 		return (ERROR);
// 	c.location.push_back(loc);
// 	return (SUCCESS);
// }

// /*
// ** True if we have a location, false otherwise
// */
// int		find_location(std::string &line)
// {
// 	std::list<std::string> tmp;

// 	tmp = split_in_list(line, " \t");
// 	if ((tmp.size() == 3 || tmp.size() == 4) && tmp.front() == "location" && tmp.back() == "{")
// 		return (true);
// 	return (false);

// }

// /*
// ** Init conf struct
// */
// void	init_config(t_config &conf)
// {
// 	conf.host = "127.0.0.1";
// 	conf.name_server = "Webserv";
// 	conf.root = "frontend/";
// 	conf.body_size_limit = 0;
// 	conf.default_server = false;
// }

// /*
// ** Parsinf of the server section
// */
// int		parse_serv(std::ifstream &fd, std::list<t_config> &conf)
// {
// 	std::list<std::string>	tmp;
// 	std::string				reader;
// 	int						open_brackets = 1;
// 	t_config				c;

// 	init_config(c);
// 	while (std::getline(fd, reader))
// 	{
// 		tmp = split_in_list(reader, " \t\n\r\v\f");
// 		if (find_end_bracket(tmp))
// 		{
// 			open_brackets -= 1;
// 			break;
// 		}
// 		else if (find_location(reader))
// 			parse_location(fd, c, reader);
// 		else if (find_config_elem(tmp, "root"))
// 			conf_get_str(c.root, tmp);
// 		else if (find_config_elem(tmp, "name_server"))
// 			conf_get_str(c.name_server, tmp);
// 		else if (find_config_elem(tmp, "host"))
// 			conf_get_str(c.host, tmp);
// 		else if (find_config_elem(tmp, "error_page"))
// 			conf_get_str(c.error_page, tmp);
// 		else if (find_config_elem(tmp, "port"))
// 			conf_get_list(c.port, tmp);
// 		else if (find_config_elem(tmp, "index"))
// 			conf_get_list(c.index, tmp);
// 		else if (find_config_elem(tmp, "body_size_limit"))
// 			conf_get_num(c.body_size_limit, tmp);
// 	}
// 	if (open_brackets != 0)
// 		return (ERROR);
// 	conf.insert(conf.end(), c);
// 	if (!conf.empty())
// 		conf.front().default_server = true;
// 	return (SUCCESS);
// }

// bool	find_server(std::string &line)
// {
// 	std::list<std::string>  spliter;
// 	size_t					size;

// 	spliter = split_in_list(line, " \t");
// 	size = spliter.size();
// 	if (size < 1 || size > 2)
// 		return (false);
// 	if (size == 2 && spliter.front() == "server" && spliter.back() == "{")
// 		return (true);
// 	return (false);
// }

// int		parse_conf(std::string path, std::list<t_config> &conf)
// {
// 	std::ifstream	fd(path.c_str());
// 	std::string		reader;

// 	if (!fd)
// 	{
// 		fd.close();
// 		std::cout << "No such file or directory." << std::endl;
// 		return (ERROR);
// 	}
// 	while (std::getline(fd, reader))
// 	{
// 		if (find_server(reader))
// 			parse_serv(fd, conf);
// 	}
// 	fd.close();
// 	return (SUCCESS);
// }

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
** parsing cgi
*/
int		parse_cgi(t_loc &l, std::list<std::string> line)
{
	l.cgi.active = true;
	if (find_config_elem(line, "AUTH_TYPE"))
		conf_get_str(l.cgi.AUTH_TYPE, line);
	else if (find_config_elem(line, "CONTENT_LENGTH"))
		conf_get_str(l.cgi.CONTENT_LENGTH, line);
	else if (find_config_elem(line, "CONTENT_TYPE"))
		conf_get_str(l.cgi.CONTENT_TYPE, line);
	else if (find_config_elem(line, "GATEWAY_INTERFACE"))
		conf_get_str(l.cgi.GATEWAY_INTERFACE, line);
	else if (find_config_elem(line, "PATH_INFO"))
		conf_get_str(l.cgi.PATH_INFO, line);
	else if (find_config_elem(line, "PATH_TRANSLATED"))
		conf_get_str(l.cgi.PATH_TRANSLATED, line);
	else if (find_config_elem(line, "QUERY_STRING"))
		conf_get_str(l.cgi.QUERY_STRING, line);
	else if (find_config_elem(line, "REMOTE_ADDR"))
		conf_get_str(l.cgi.REMOTE_ADDR, line);
	else if (find_config_elem(line, "REMOTE_IDENT"))
		conf_get_str(l.cgi.REMOTE_IDENT, line);
	else if (find_config_elem(line, "REMOTE_USER"))
		conf_get_str(l.cgi.REMOTE_USER, line);
	else if (find_config_elem(line, "REQUEST_METHOD"))
		conf_get_str(l.cgi.REQUEST_METHOD, line);
	else if (find_config_elem(line, "REQUEST_URI"))
		conf_get_str(l.cgi.REQUEST_URI, line);
	else if (find_config_elem(line, "SCRIPT_NAME"))
		conf_get_str(l.cgi.SCRIPT_NAME, line);
	else if (find_config_elem(line, "SERVER_NAME"))
		conf_get_str(l.cgi.SERVER_NAME, line);
	else if (find_config_elem(line, "SERVER_PORT"))
		conf_get_str(l.cgi.SERVER_PORT, line);
	else if (find_config_elem(line, "SERVER_PROTOCOL"))
		conf_get_str(l.cgi.SERVER_PROTOCOL, line);
	else if (find_config_elem(line, "SERVER_SOFTWARE"))
		conf_get_str(l.cgi.SERVER_SOFTWARE, line);
	return (SUCCESS);
}

/*
** Init location values
*/
void	init_location(t_config &conf, t_loc &loc)
{
	loc.body_size_limit = conf.body_size_limit;
}

/*
** Find if the line is a comment
*/
bool	find_comment(std::list<std::string> &line)
{
	if (line.empty())
		return (true);
	if (line.front().size() > 0)
	{
		if (line.front()[0] == '#')
			return (true);
	}
	return (false);
}

/*
** parse the location in config file
*/
int		parse_location(std::ifstream &fd, t_config &c, std::string &line)
{
	std::string				reader;
	t_loc					loc;
	std::list<std::string>	tmp;
	int						brackets = 1;

	init_location(c, loc);
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
			conf_get_list(loc.http_methods, tmp);
		else if (find_config_elem(tmp, "index"))
			conf_get_list(loc.index, tmp);
		else if (find_config_elem(tmp, "body_size_limit"))
			conf_get_num(loc.body_size_limit, tmp);
		else if (find_config_elem(tmp, "directory_files_search"))
			conf_get_str(loc.directory_files_search, tmp);
		else if (find_config_elem(tmp, "directory_listing"))
			conf_get_str(loc.directory_listing, tmp);
		else if (find_config_elem(tmp, "default_file_directory_request"))
			conf_get_str(loc.default_file_directory_request, tmp);
		else if (find_config_elem(tmp, "file_upload_location"))
			conf_get_str(loc.upload_files_location, tmp);
		else if (find_config_elem(tmp, "fastcgi_param"))
			parse_cgi(loc, tmp);
		else
		{
			if (!find_comment(tmp))
			{
				return (ERROR);
			}
		}
	}
	if (brackets != 0)
		return (ERROR);
	c.location.push_back(loc);
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
** Init conf struct
*/
void	init_config(t_config &conf)
{
	// conf.host = "127.0.0.1";
	conf.name_server = "Webserv";
	conf.root = "frontend/";
	conf.body_size_limit = 0;
	conf.default_server = false;
}

/*
** Copy a and return the copied conf
*/
void	copy_conf(std::list<t_config> &conf, t_config &srcs, size_t len)
{
	t_config new_conf;
	size_t i = 0;

	new_conf.host = srcs.host;
	new_conf.root = srcs.root;
	new_conf.name_server = srcs.name_server;
	new_conf.error_page = srcs.error_page;
	std::list<t_loc>::iterator it_loc = srcs.location.begin();
	while (it_loc != srcs.location.end())
	{
		t_loc	new_loc;
		copy_loc(new_loc, *it_loc);
		new_conf.location.push_back(new_loc);
		++it_loc;
	}
	new_conf.port = srcs.port;
	while (i < len)
	{
		new_conf.port.pop_front();
		++i;
	}
	new_conf.index = srcs.index;
	new_conf.error_page = srcs.error_page;
	new_conf.body_size_limit = srcs.body_size_limit;
	new_conf.default_server = false;
	conf.insert(conf.end(), new_conf);
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

	init_config(c);
	while (std::getline(fd, reader))
	{
		tmp = split_in_list(reader, " \t\n\r\v\f");
		if (find_end_bracket(tmp))
		{
			open_brackets -= 1;
			break;
		}
		else if (find_location(reader))
		{
			if (parse_location(fd, c, reader))
			{
				return (ERROR);
			}
		}
		else if (find_config_elem(tmp, "root"))
			conf_get_str(c.root, tmp);
		else if (find_config_elem(tmp, "name_server"))
			conf_get_str(c.name_server, tmp);
		else if (find_config_elem(tmp, "host"))
			conf_get_str(c.host, tmp);
		else if (find_config_elem(tmp, "error_page"))
			conf_get_str(c.error_page, tmp);
		else if (find_config_elem(tmp, "port"))
			conf_get_list(c.port, tmp);
		else if (find_config_elem(tmp, "index"))
			conf_get_list(c.index, tmp);
		else if (find_config_elem(tmp, "body_size_limit"))
			conf_get_num(c.body_size_limit, tmp);
		else
		{
			if (!find_comment(tmp))
			{
				return (ERROR);
			}
		}
	}
	if (open_brackets != 0)
	{
		return (ERROR);
	}
	conf.insert(conf.end(), c);
	if (!conf.empty())
		conf.front().default_server = true;
	if (c.port.size() > 1)
	{
		size_t len = 1;
		while (len < c.port.size())
		{
			copy_conf(conf, c, len);
			++len;
		}
	}
	return (SUCCESS);
}

int	find_server(std::string &line)
{
	std::list<std::string>  spliter;
	size_t					size;

	spliter = split_in_list(line, " \t");
	size = spliter.size();
	if (size < 1)
		return (1);
	if (size == 2 && spliter.front() == "server" && spliter.back() == "{")
		return (0);
	if (find_comment(spliter))
		return (1);
	return (ERROR);
}

int		parse_conf(std::string path, std::list<t_config> &conf)
{
	std::ifstream	fd(path.c_str());
	std::string		reader;
	int				ret;

	if (!fd)
	{
		fd.close();
		return (2);
	}
	while (std::getline(fd, reader))
	{
		if (!(ret = find_server(reader)))
		{
			if (parse_serv(fd, conf))
			{
				fd.close();
				return (ERROR);
			}
		}
		else
		{
			if (ret == -1)
			{
				fd.close();
				return (ERROR);
			}
		}
	}
	fd.close();
	return (SUCCESS);
}