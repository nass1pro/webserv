/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 16:44:30 by judecuyp          #+#    #+#             */
/*   Updated: 2021/07/14 10:27:47 by judecuyp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config.hpp"
#include "../include/utils.hpp"

/*
** Return the index where the body starts in the full request.
** Return -1 if we can't find 2 \n in the request
*/
int		get_body_index(t_req &req)
{
	size_t i = 0;
	size_t size = req.full_req.size();

	if (size == 0)
		return (-1);
	while ((req.full_req[i] == '\t' || req.full_req[i] == '\n' || req.full_req[i] == '\r' || req.full_req[i] == '\v' || req.full_req[i] == '\f') && i < size)
		i++;
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
		else if (find_field_name(lines.front(), "x-secret"))
		{
			req.header.Secret_req.push_back(lines.front());
		}
		lines.pop_front();
	}
}

/*
** Set error code to 405 if the method isnt correct
*/
int		check_method(t_req &req)
{
	if (!req.location.http_methods.empty())
	{
		std::list<std::string>::iterator it = req.location.http_methods.begin();
		while (it != req.location.http_methods.end())
		{
			if (req.method == *it)
			{
				return (0);
			}
			++it;
		}
		return ((req.error = 405));
		
	}
	else
	{
		if (req.method != "PUT" && req.method != "GET" && req.method != "POST" && req.method != "HEAD" /*&& req.method != "DELETE"*/)
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
	std::list<std::string>	split;
	std::string 			line;

	if (lines.empty())
	{
		return (-2);
	}
	
	line = lines.front();	
	if (line.find("HTTP/1.1", 0) == std::string::npos || line.find(" ", 0) == 0)
	{
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
void	get_body(t_req &req/*, t_config &conf*/)
{
	//(void)conf;
	//std::cout << req.full_req.size() << " | " << req.body_index << std::endl;
	if (req.body_index != req.full_req.size())
	{
		req.body_content.clear();
		//std::cout << req.full_req.size() << " | " << req.body_index << std::endl;
		req.body_content = req.full_req.substr(req.body_index, req.full_req.size() - req.body_index);
	}
	
}

/*
** Parse the body and cut 1 / 2 lines if the type is 'chuncked'
*/
void	parse_body(std::string &body)
{
		std::vector<std::string>	line_of_body;
		std::string					line;
		const size_t				size_body = body.size();
		size_t						size_line_of_body;
		size_t						i = 0;

		for (size_t j = 0; j < size_body; j++)
		{
			if (j == 0 && body[j] == '\n')
				continue ;
			else if (body[j] == '\n')
			{
				line_of_body.push_back(line);
				line.clear();
			}
			else
				line += body[j];
		}
		std::vector<std::string>::iterator it = line_of_body.begin();
		size_line_of_body = line_of_body.size();
		body.clear();
		body.reserve(100000010);
		for ( ; it != line_of_body.end(); it++)
		{
			if (*it == "0")
				break ;
			else if (i++ % 2)
			{
				body += (*it).erase((*it).size() - 1);
			}
		}
}

/*
** main function for the parsing of the request
** (take the _req struct as parameter with full_request field filled)
*/
int		parse_request(std::map<int, t_req>::iterator &client, /*t_req &req,*/ t_config &conf)
{

	std::list<std::string> list_lines;
	int ret = 0;

	//std::cout << conf.serv.req[client->first].full_req << std::endl;
	
	init_request(conf.serv.req[client->first]);
	if ((conf.serv.req[client->first].body_index = get_body_index(conf.serv.req[client->first])) == -1)
	{
		conf.serv.req[client->first].done = false;
		return (ERROR);
	}
	list_lines = split_in_list(conf.serv.req[client->first].full_req.substr(0, conf.serv.req[client->first].body_index/*req.body_index*/), "\t\n\r\v\f");
	if ((ret = parse_first_line(conf.serv.req[client->first], list_lines, conf)) < 0)
	{
		if (ret == -2)
			conf.serv.req[client->first].done = false;
		else
			conf.serv.req[client->first].done = true;
		return (ERROR);
	}
	parse_header(conf.serv.req[client->first], list_lines);

	get_body(conf.serv.req[client->first]/*, conf*/);
	if (conf.serv.req[client->first].header.Content_Length.empty() == true && conf.serv.req[client->first].header.Transfer_Encoding.empty() == true && conf.serv.req[client->first].method == "POST")
	{
		conf.serv.req[client->first].error = 405;
		conf.serv.req[client->first].done = true;
		return (ERROR);
	}
	else if (conf.serv.req[client->first].header.Transfer_Encoding == "chunked")
	{
		int i = 0;
		
		if (conf.serv.req[client->first].body_content.empty())
		{
			conf.serv.req[client->first].done = false;
			return (ERROR);
		}

		i = conf.serv.req[client->first].body_content.size() - 1;
		while(is_white_space(conf.serv.req[client->first].body_content[i]) && i >= 0)
		{
			i--;
		}
		if ((i >= 1 && conf.serv.req[client->first].body_content[i] == '0' && conf.serv.req[client->first].body_content[i - 1] == '\n') || (i == 0 && conf.serv.req[client->first].body_content[i] == '0'))
		{
			conf.serv.req[client->first].done = true;
		}
		else
		{
			conf.serv.req[client->first].done = false;
			return (ERROR);
		}
	}
	else if (conf.serv.req[client->first].header.Content_Length.empty() == false)
	{
		if ((size_t)atoi(conf.serv.req[client->first].header.Content_Length.c_str()) != conf.serv.req[client->first].body_content.size())
		{
			conf.serv.req[client->first].done = false;
			return (ERROR);
		}
	}
	if (conf.serv.req[client->first].header.Transfer_Encoding == "chunked")
		parse_body(conf.serv.req[client->first].body_content);
	if (conf.serv.req[client->first].location.body_size_limit > 0)
	{
		if (conf.serv.req[client->first].body_content.size() > conf.serv.req[client->first].location.body_size_limit)
		{
			conf.serv.req[client->first].error = 413;
			conf.serv.req[client->first].done = true;
			return (ERROR);
		}
	}
	return (SUCCESS);
}
