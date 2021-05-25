/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_url.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 13:05:49 by judecuyp          #+#    #+#             */
/*   Updated: 2021/05/25 13:05:49 by judecuyp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config.hpp"
#include "../include/utils.hpp"

/*
** TODO parsing url
** 
*/

/*
** Copy a t_loc into another t_loc
*/
void	copy_loc(t_loc &dest, t_loc &copy)
{
	dest.location_match = copy.location_match;
	dest.optional_modifier = copy.optional_modifier;
	dest.http_methods = copy.http_methods;
	dest.body_size_limit = copy.body_size_limit;
	dest.directory_files_search = copy.directory_files_search;
	dest.directory_listing = copy.directory_listing;
	dest.default_file_directory_request = copy.default_file_directory_request;
	dest.upload_files_location = copy.upload_files_location;
	
	std::list<std::string>::iterator it = copy.index.begin();
	while (it != copy.index.end())
	{
		dest.index.push_back(*it);
		++it;
	}
	
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
** Return the path without the file
*/
//std::stiring

/*
** Put an error code in req->error if we can't find the file
*/
/*bool	find_file(t_req &req)
{
	std::string path_no_files = find_path(t_req->url);
}*/

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
	//P(new_url); /////////////teeestststts
	if (new_url.find_last_of("/") == new_url.size() - 1)
	{
		if (!loc.index.empty())
			new_url.append(loc.index.front());
	}
	P(new_url); /////////////////////PRINT TREST
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
		{
			std::cout << "FOUND" << std::endl;
			found = 1;
		}
		if (!found)
			++it;
	}
	//P("test");
	if (!found)
		return ; // check retourner code erreur ou jsp
	copy_loc(req_loc, *it);
	req.location = req_loc;
	req.url = create_local_path(req, req.location, conf);
	P(req.url);
	//find_dir(req);
}