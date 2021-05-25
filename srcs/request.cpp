/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 16:44:30 by judecuyp          #+#    #+#             */
/*   Updated: 2021/05/25 11:43:58 by judecuyp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config.hpp"
#include "../include/utils.hpp"

/*
** TODO :
**
*/

/*
** Return the index where the body starts in the full request.
** Return -1 if we can't find 2 \n in the request
*/
int		get_body_index(std::string full_req)
{
	size_t i = 0;
	size_t size = full_req.size();

	if (size == 0)
		return (-1);
	while (i < size)
	{
		if (full_req[i] == '\n')
		{
			++i;
			while (is_in_charset(full_req[i], "\t\r\v\f") && i < size)
				i++;
			if (i < size && full_req[i] == '\n')
				return (++i);
		}
		else
			++i;
	}
	return (ERROR);
}

/*
** True if the start of the line is the name of the field required, false otherwise
** (we can erase the field name here but I don't. Like that we're allowed to use this function later in other cases)
*/
bool	find_field_name(std::string &line, std::string const &name)
{
	std::string tester;

	tester = line.substr(0, name.size());
	for (size_t i = 0; i < tester.length(); i++)
	{
		tester[i] = tolower(tester[i]);
	}
	if (tester == name)
		return (true);
	else
		return (false);

}

/*
** take the list corresponding at the right field, the line to split in him and the name of the field.
** first remove the name_field of the line
** Split the line and insert values in the list (fidelds = req->header->[field_name])
*/
int		split_fields(std::list<std::string> &fields, std::string &line, std::string const &field_name)
{
	std::list<std::string> split;

	line.erase(0, field_name.size());
	split = split_in_list(line, " ,");
	//print_list(split); ///////////////////////////////////////////////
	fields.insert(fields.end(), split.begin(), split.end());
	return (SUCCESS);
}

/*
** Same concept than split_fields but put all the line in a simple string (less the field name)
*/
int		split_fields_str(std::string &field, std::string &line, std::string const &field_name)
{
	line.erase(0, field_name.size());
	field = line;
	return (SUCCESS);
}

/*
** Parse the header
** find the good header field and split the values into req->header->[header_field]
*/
void	parse_header(t_req &req, std::list<std::string> &lines)
{

	while (!lines.empty())
	{
		//print_list(lines); //////////////////////////////////////////////////////////////////////
		if (find_field_name(lines.front(), "accept-charsets:"))
			split_fields_str(req.header.Accept_Charsets, lines.front(), "accept-charsets:");
		else if (find_field_name(lines.front(), "accept-languages:"))
			split_fields(req.header.Accept_Language, lines.front(), "accept-languages:");
		else if (find_field_name(lines.front(), "allow:"))
			split_fields_str(req.header.Allow, lines.front(), "allow:");
		else if (find_field_name(lines.front(), "authorization:"))
			split_fields(req.header.Authorization, lines.front(), "authorization:");
		else if (find_field_name(lines.front(), "content_language:"))
			split_fields_str(req.header.Content_Language, lines.front(), "content_language:");
		else if (find_field_name(lines.front(), "content_length:"))
			split_fields_str(req.header.Content_Length, lines.front(), "content_length:");
		else if (find_field_name(lines.front(), "content_location:"))
			split_fields_str(req.header.Content_Location, lines.front(), "content_location:");
		else if (find_field_name(lines.front(), "content_type:"))
			split_fields_str(req.header.Content_Type, lines.front(), "content_type:");
		else if (find_field_name(lines.front(), "date:"))
			split_fields_str(req.header.Date, lines.front(), "date:");
		else if (find_field_name(lines.front(), "host:"))
			split_fields(req.header.Host, lines.front(), "host:");
		else if (find_field_name(lines.front(), "last_modified:"))
			split_fields_str(req.header.Last_modified, lines.front(), "last_modified:");
		else if (find_field_name(lines.front(), "referer:"))
			split_fields(req.header.Referer, lines.front(), "referer:");
		else if (find_field_name(lines.front(), "retry_after:"))
			split_fields_str(req.header.retry_after, lines.front(), "retry_after:");
		else if (find_field_name(lines.front(), "server:"))
			split_fields_str(req.header.Server, lines.front(), "server:");
		else if (find_field_name(lines.front(), "transfer_encoding:"))
			split_fields_str(req.header.Transfer_Encoding, lines.front(), "transfer_encoding:");
		else if (find_field_name(lines.front(), "user_agent:"))
			split_fields(req.header.User_Agent, lines.front(), "user_agent:");
		else if (find_field_name(lines.front(), "www_authenticate:"))
			split_fields_str(req.header.WWW_Authenticate, lines.front(), "www_authenticate:");
		else
			std::cout << "Not implemented" << std::endl; // A voir comment gérér les autres cas
		lines.pop_front();
	}
	//req.header = h;
	//return (h);
}

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
	dest.index = copy.index;
	
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
** TODO parsing url
** 
*/

/*
** Create a local path with root etc
*/
std::string		create_local_path(std::string &url, t_loc &loc)
{
	std::string new_url;

	new_url = url.substr(loc.location_match.size());
	new_url.insert(0, loc.directory_files_search);
	
	if (new_url.find_last_of("/") == new_url.size() - 1)
		new_url.insert(new_url.size(), loc.index.front());
	P(new_url);
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
	copy_loc(req_loc, *it);
	req.location = req_loc;
	if (req_loc.directory_files_search != "")
		req.url = create_local_path(req.url, req.location);	
}

/*
** Take the first line of the request split her in a list,
** add values in the struct t_req and delete used content to the lines list
** return negative value in case of error
*/
int		parse_first_line(t_req &req, std::list<std::string> &lines, t_config &conf)
{
	std::string 			line(lines.front());
	std::list<std::string>	split;

	if (line.find("HTTP/1.1", 0) == std::string::npos || line.find(" ", 0) == 0)
		return (ERROR);
	split = split_in_list(line, " ");
	req.method = split.front();
	split.pop_front();

	req.url = split.front();
	split.pop_front();

	req.version = split.front();
	lines.pop_front();

	get_req_location(req, conf);
	print_location(&req.location);
	return (SUCCESS);
}

/*
** Initialise struct t_req
*/
void	init_request(t_req &req)
{
	req.body_index = 0;
	req.done = false;
}

/*
** Put all the body into req->body_content
*/
void	get_body(t_req &req)
{
	if (req.body_index != req.full_req.size())
		req.body_content = req.full_req.substr(req.body_index, req.full_req.size() - req.body_index);
}

/*
** main function for the parsing of the request
** (take the _req struct as parameter with full_request field filled)
** for the moment return a negative number in case of weird behavior (!!) modifier retours erreurs etc
*/
int		parse_request(t_req &req, t_config &conf)
{
	std::list<std::string> list_lines;

	//conf.default_server = false;
	init_request(req);
	if ((req.body_index = get_body_index(req.full_req)) == -1)
		return (ERROR);
	list_lines = split_in_list(req.full_req.substr(0, req.body_index), "\t\n\r\v\f");
	//print_list(list_lines); //test
	if (parse_first_line(req, list_lines, conf) < 0)
		return (ERROR);
	//std::cout << req.full_req << std::endl;
	parse_header(req, list_lines);
	//std::cout<< "fini"<<std::endl;
	get_body(req);
	req.done = true;
	return (SUCCESS);
}
