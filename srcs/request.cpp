/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 16:44:30 by judecuyp          #+#    #+#             */
/*   Updated: 2021/06/02 19:08:13 by judecuyp         ###   ########.fr       */
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
int		get_body_index(t_req &req/*std::string full_req*/)
{
	size_t i = 0;
	size_t size = req.full_req.size();

	if (size == 0)
		return (-1);
	while (i < size)
	{
		if (req.full_req[i] == '\n')
		{
			++i;
			while (is_in_charset(req.full_req[i], "\t\r\v\f") && i < size)
				i++;
			if (i < size && req.full_req[i] == '\n')
			{
				req.done = true;
				return (++i);
			}
		}
		else
			++i;
	}
	return (-1);
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
		if (find_field_name(lines.front(), "accept-charsets: "))
			split_fields_str(req.header.Accept_Charsets, lines.front(), "accept-charsets: ");
		else if (find_field_name(lines.front(), "accept-languages: "))
			split_fields(req.header.Accept_Language, lines.front(), "accept-languages: ");
		else if (find_field_name(lines.front(), "allow: "))
			split_fields_str(req.header.Allow, lines.front(), "allow: ");
		else if (find_field_name(lines.front(), "authorization: "))
			split_fields(req.header.Authorization, lines.front(), "authorization: ");
		else if (find_field_name(lines.front(), "content-language: "))
			split_fields_str(req.header.Content_Language, lines.front(), "content-language: ");
		else if (find_field_name(lines.front(), "content-length: "))
			split_fields_str(req.header.Content_Length, lines.front(), "content-length: ");
		else if (find_field_name(lines.front(), "content-location: "))
			split_fields_str(req.header.Content_Location, lines.front(), "content-location: ");
		else if (find_field_name(lines.front(), "content-type: "))
			split_fields_str(req.header.Content_Type, lines.front(), "content-type: ");
		else if (find_field_name(lines.front(), "date: "))
			split_fields_str(req.header.Date, lines.front(), "date: ");
		else if (find_field_name(lines.front(), "host: "))
			split_fields(req.header.Host, lines.front(), "host: ");
		else if (find_field_name(lines.front(), "last-modified: "))
			split_fields_str(req.header.Last_modified, lines.front(), "last-modified: ");
		else if (find_field_name(lines.front(), "referer: "))
			split_fields(req.header.Referer, lines.front(), "referer: ");
		else if (find_field_name(lines.front(), "retry-after: "))
			split_fields_str(req.header.retry_after, lines.front(), "retry-after: ");
		else if (find_field_name(lines.front(), "server: "))
			split_fields_str(req.header.Server, lines.front(), "server: ");
		else if (find_field_name(lines.front(), "transfer-encoding: "))
		{
			split_fields_str(req.header.Transfer_Encoding, lines.front(), "transfer-encoding: ");
		}
		else if (find_field_name(lines.front(), "user-agent: "))
			split_fields(req.header.User_Agent, lines.front(), "user-agent: ");
		else if (find_field_name(lines.front(), "www-authenticate: "))
			split_fields_str(req.header.WWW_Authenticate, lines.front(), "www-authenticate: ");
		// else
			// std::cout << "Not implemented" << std::endl; // A voir comment gérér les autres cas
		lines.pop_front();
	}
	//req.header = h;
	//return (h);
}

/*
** Set error code to 405 if the method isnt correct
*/
int		check_method(t_req &req)
{
	//std::cout << ":ETHOD ; " << req.location.http_methods << std::endl;
	if (!req.location.http_methods.empty())
	{
		std::list<std::string>::iterator it = req.location.http_methods.begin();
		while (it != req.location.http_methods.end())
		{
			//std::cout << " ICI meHTHOD : " << *it << std::endl;
			if (req.method == *it)
				return (0);
			++it;
		}
		return ((req.error = 405));
		
	}
	else
	{
		if (req.method != "PUT" && req.method != "GET" && req.method != "POST" && req.method != "HEAD" && req.method != "DELETE")
			return ((req.error = 405));
	}
	return (0);
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

	// std::cout<< req.body_index << " celui la"<< req.full_req << std::endl;

	if (line.find("HTTP/1.1", 0) == std::string::npos || line.find(" ", 0) == 0)
	{
		// P("je suis laaaaaa 1")
		// std::cout << "---"<< line << std::endl;
		req.error = 400;
		return (ERROR);
	}
	split = split_in_list(line, " ");
	if (split.size() != 3)
	{
		req.error = 400;
		return (ERROR);
	}
	req.method = split.front();
	split.pop_front();

	req.url = split.front();
	split.pop_front();

	req.version = split.front();
	lines.pop_front();

	get_req_location(req, conf);
	if (req.error != 0)
		return (ERROR);
	if (check_method(req) != 0)
		return (ERROR);
	return (SUCCESS);
}

/*
** Initialise struct t_req
*/
void	init_request(t_req &req)
{
	req.body_index = 0;
	req.done = false;
	req.error = 0;
}

/*
** Put all the body into req->body_content
*/
void	get_body(t_req &req, t_config &conf)
{
	(void)conf;
	if (req.body_index != req.full_req.size())
		req.body_content = req.full_req.substr(req.body_index, req.full_req.size() - req.body_index);
	
}

/*
** main function for the parsing of the request
** (take the _req struct as parameter with full_request field filled)
** for the moment return a negative number in case of weird behavior (!!) modifier retours erreurs etc
*/
int		parse_request(std::map<int, t_req>::iterator &client, t_req &req, t_config &conf)
{
	std::list<std::string> list_lines;

	// std::cout<< "recois \n " << conf.serv.req[client->first].full_req << "client : " << client->first << "|| \n" <<std::endl;
	init_request(conf.serv.req[client->first]);
	if ((conf.serv.req[client->first].body_index = get_body_index(conf.serv.req[client->first]/*.full_req*/)) == -1)
	{
		//req.error = 400;
		// std::cout << "Incomplete request." << std::endl;
		conf.serv.req[client->first].done = false;
		return (ERROR);
	}
	list_lines = split_in_list(conf.serv.req[client->first].full_req.substr(0, req.body_index), "\t\n\r\v\f");
	if (parse_first_line(conf.serv.req[client->first], list_lines, conf) < 0)
	{
		/*if (conf.serv.req[client->first].body_content.empty() && conf.serv.req[client->first].method == "POST")
		{
			// std::cout<< "laaaaaaa"<< std::endl;
			conf.serv.req[client->first].done = false;
			return (ERROR);
		}*/
		//else
			conf.serv.req[client->first].done = true;
		return (ERROR);
	}
	parse_header(conf.serv.req[client->first], list_lines);
	get_body(conf.serv.req[client->first], conf);
	//std::cout<< std::endl << conf.serv.req[client->first].full_req << std::endl << std::endl;
	//std::cout<< conf.serv.req[client->first].header.Content_Length << "|content_length"<< std::endl;
	//std::cout<< conf.serv.req[client->first].method << "|POST"<< std::endl;
	//std::cout<< conf.serv.req[client->first].header.Transfer_Encoding << "|transs"<< std::endl;


	if (conf.serv.req[client->first].header.Content_Length.empty() == true && conf.serv.req[client->first].header.Transfer_Encoding.empty() == true && conf.serv.req[client->first].method == "POST")
	{
		//std::cout<< "laaaaaaa"<< std::endl;
		req.error = 405;
		conf.serv.req[client->first].done = true;
		return (ERROR);
	}
	else if (conf.serv.req[client->first].header.Transfer_Encoding == "chunked")
	{
		// std::cout<< "laaaaaaa"<< std::endl;
		int i = 0;
		i = conf.serv.req[client->first].body_content.size() - 1;
		// std::cout<< i << "\n" << std::endl;
		if (conf.serv.req[client->first].body_content.empty())
		{
			conf.serv.req[client->first].done = false;
			return (ERROR);
		}
		while(is_white_space(conf.serv.req[client->first].body_content[i]))
		{
			i--;
		}
		if (conf.serv.req[client->first].body_content[i] == '0' && conf.serv.req[client->first].body_content[i - 1] == '\n')
		{
			conf.serv.req[client->first].done = true;
		}
		else
		{
			// std::cout <<
			conf.serv.req[client->first].done = false;
			return (ERROR);
		}
	}
	size_t size = 0;
	size = req.location.body_size_limit/*conf.body_size_limit*/ * (size_t)1001000;
//	std::cout << "EST cE QUON A DES CGI PAR ICI ?????? --> " << req.location.cgi.SCRIPT_NAME << std::endl;
//	std::cout << "EST cE QUON A DES CGI ACTIIIIIIVES PAR ICI ?????? --> " << req.location.cgi.active << std::endl;
	//std::cout << req.body_content.size()  <<std::endl;
	// if (req.body_content.size() > 10000000)
	// 	req.error = 413;
	// if (req.error != 0)
	// {
	// 	req.done = true;
	// 	return (ERROR);
	// }
	//req.done = true;
	return (SUCCESS);
}
