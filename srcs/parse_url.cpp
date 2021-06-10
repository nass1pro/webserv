/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_url.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 13:05:49 by judecuyp          #+#    #+#             */
/*   Updated: 2021/06/10 12:00:38 by judecuyp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.hpp"

/*
** Copy a t_loc into another t_loc
*/
void	copy_loc(t_loc &dest, t_loc &copy)
{
	dest.location_match = copy.location_match;
	dest.optional_modifier = copy.optional_modifier;
	dest.http_methods.clear();
	std::list<std::string>::iterator itme = copy.http_methods.begin();
	while (itme != copy.http_methods.end())
	{
		dest.http_methods.push_back(*itme);
		++itme;
	}
	dest.body_size_limit = copy.body_size_limit;
	dest.directory_files_search = copy.directory_files_search;
	dest.directory_listing = copy.directory_listing;
	dest.default_file_directory_request = copy.default_file_directory_request;
	dest.upload_files_location = copy.upload_files_location;
	
	dest.index.clear();
	std::list<std::string>::iterator it = copy.index.begin();
	while (it != copy.index.end())
	{
		dest.index.push_back(*it);
		++it;
	}
	dest.cgi.active = copy.cgi.active;
	dest.cgi.AUTH_TYPE = copy.cgi.AUTH_TYPE;
	dest.cgi.CONTENT_LENGTH = copy.cgi.CONTENT_LENGTH;
	dest.cgi.CONTENT_TYPE = copy.cgi.CONTENT_TYPE;
	dest.cgi.GATEWAY_INTERFACE = copy.cgi.GATEWAY_INTERFACE;
	dest.cgi.PATH_INFO = copy.cgi.PATH_INFO;
	dest.cgi.PATH_TRANSLATED = copy.cgi.PATH_TRANSLATED;
	dest.cgi.QUERY_STRING = copy.cgi.QUERY_STRING;
	dest.cgi.REMOTE_ADDR = copy.cgi.REMOTE_ADDR;
	dest.cgi.REMOTE_IDENT = copy.cgi.REMOTE_IDENT;
	dest.cgi.REMOTE_USER = copy.cgi.REMOTE_USER;
	dest.cgi.REQUEST_METHOD = copy.cgi.REQUEST_METHOD;
	dest.cgi.REQUEST_URI = copy.cgi.REQUEST_URI;
	dest.cgi.SCRIPT_NAME = copy.cgi.SCRIPT_NAME;
	dest.cgi.SERVER_NAME = copy.cgi.SERVER_NAME;
	dest.cgi.SERVER_PORT = copy.cgi.SERVER_PORT;
	dest.cgi.SERVER_PROTOCOL = copy.cgi.SERVER_PROTOCOL;
	dest.cgi.SERVER_SOFTWARE = copy.cgi.SERVER_SOFTWARE;
}

/*
** Check if the index is in the directory and add index filename in req.url
*/
bool	check_and_add_index(std::list<std::string> &files_list, t_req &req)
{
	std::list<std::string>::iterator	it_index = req.location.index.begin();
	std::list<std::string>::iterator	it;
	while (it_index != req.location.index.end())
	{
		it = files_list.begin();
		while (it != files_list.end())
		{
			if (*it == *it_index)
			{
				req.url.append(*it_index);
				return (true);
			}
			++it;
		}
		++it_index;
	}
	return (false);
}

/*
** Check if the filename is a directory
*/
bool	url_is_dir(std::string &filename, std::list<std::string> &files_list)
{
	DIR						*directory;
	struct dirent			*file;

	if ((directory = opendir(filename.c_str())) == 0)
		return (false);
	while ((file = readdir(directory)) != 0)
	{
		if (file->d_type != DT_DIR)
			files_list.push_back(file->d_name);
	}
	closedir(directory);
	return (true);
}

/*
** Return the path without the file
*/
bool	check_files_in_directory(std::list<std::string> &files_list, t_req &req, std::string &no_file_path)
{
	std::string							file;
	std::list<std::string>::iterator	it;
	std::list<std::string>				new_list;

	if (no_file_path != req.url)
	{
		file = req.url.substr(no_file_path.size());
		it = files_list.begin();
		while (it != files_list.end())
		{
			if (*it == file)
			{
				if (!url_is_dir(req.url, new_list))
					return (true);
				else
				{
					req.url.append("/");
					if (check_and_add_index(new_list, req))
						return (true);
				}
			}
			++it;
		}
		return (false);
	}
	else
	{
		if (check_and_add_index(files_list, req))
			return (true);
		return (false);
	}
}

/*
** Put an error code in req->error if we can't find the file
*/
bool	find_dir(t_req &req)
{
	std::list<std::string>	files_list;
	std::string				no_file_path = req.url;
	struct dirent			*file;
	DIR						*directory;
	if (no_file_path.empty() || (!no_file_path.empty() && no_file_path[no_file_path.size() - 1] == '/'))
	{
		if (req.location.index.empty())
		{
			req.error = 404;
			return (false);
		}
	}
	if (!no_file_path.empty() && no_file_path[no_file_path.size() - 1] != '/')
		no_file_path.erase(no_file_path.find_last_of("/") + 1);
	if ((directory = opendir(no_file_path.c_str())) == 0)
	{
		req.error = 404;
		return (false);
	}
	while ((file = readdir(directory)) != 0)
			files_list.push_back(file->d_name);
	closedir(directory);
	if (!check_files_in_directory(files_list, req, no_file_path))
	{
		req.error = 404;
		return (false);
	}
	return (true);
}

/*
** Create a local path with root etc
*/
std::string		create_local_path(t_req &req, t_loc &loc, t_config &conf)
{
	std::string new_url;
	if (!loc.directory_files_search.empty())
	{
		new_url = req.url.substr(loc.location_match.size());
		new_url.insert(0, loc.directory_files_search);
	}
	else
		new_url = req.url;
	if (!new_url.empty() && new_url[0] == '/')
		new_url.erase(0, 1);
	new_url.insert(0, conf.root);
	return (new_url);
}

/*
** find a directory location
*/
bool	find_directory(std::string &path, std::string &dir)
{
	std::string	tmp;
	size_t		end;
	end = path.find_first_of("/");
	if (path.size() > 0 && path[0] == '/')
		end = path.find_first_of("/", 1);
	if (end == std::string::npos)
		end = path.size();
	else
		++end;
	tmp = path.substr(0, end);
	if (tmp == dir)
		return (true);
	if (tmp.size() > 0 && tmp[tmp.size() - 1] != '/')
	{
		tmp.push_back('/');
		if (tmp == dir)
		{
			path.insert(path.end(), '/');
			return (true);
		}
	}
	return (false);
}

/*
**  Find if the path have a specific extention location
*/
bool	find_extention(std::string &url_path, std::string &extention_to_find, std::list<std::string> &methods, std::string &req_method)
{
	std::string tmp;
	std::list<std::string>::iterator it;

	if (url_path.size() >= extention_to_find.size())
	{
		tmp = url_path.substr(url_path.size() - extention_to_find.size(), url_path.size());
		if (tmp == extention_to_find)
		{
			it = methods.begin();
			while (it != methods.end())
			{
				if (req_method == *it)
					return (true);
				++it;
			}
			
		}
	}
	return (false);
}

/*
** check in all the location if the end of the req.url is the same as the extension location
** If we found a correct extension we check if the method is correct.
** then we copy the loctation and put the path for create a local url
*/
bool	get_ext_loc(t_req &req, t_config &conf, int found)
{
	std::list<t_loc>::iterator	it;
	t_loc						req_loc;

	it = conf.location.begin();
	while (it != conf.location.end() && found < 2)
	{
		if (!(it->optional_modifier.empty()))
		{
			if (find_extention(req.url, it->location_match, it->http_methods, req.method))
			{
				found += 1;
				break ;
			}
		}
		++it;
	}
	if (found == 2)
	{
		copy_loc(req_loc, *it);
		req_loc.location_match = req.location.location_match;
		if (req_loc.directory_files_search.empty())
			req_loc.directory_files_search = req.location.directory_files_search;
		req.location = req_loc;
		return (true);
	}
	return (false);
}

/*
** find the location asked by de request
*/
void	get_req_location(t_req &req, t_config &conf)
{
	std::list<t_loc>::iterator	it = conf.location.begin();
	int 						found = 0;
	std::string					compare = "http://" + conf.host + ":" + conf.port.front();
	t_loc						req_loc;
	if (req.url.find(compare) != std::string::npos)
		req.url = req.url.substr(compare.size());
	while (it != conf.location.end() && !found)
	{
		if (find_directory(req.url, it->location_match))
			found = 1;
		if (!found)
			++it;
	}
	if (!found)
	{
		req.error = 404;
		P("Directory not found");
		return ;
	}
	copy_loc(req_loc, *it);
	req.location = req_loc;
	get_ext_loc(req, conf, found);
	if (req.method == "PUT")
		return ;
	req.url = create_local_path(req, req.location, conf);
	if (!find_dir(req))
	{
		if (req.method == "POST")
			req.error = 0;
		else
			P("File not found");
	}
}
