/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafidi <ehafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 19:31:25 by ehafidi           #+#    #+#             */
/*   Updated: 2021/04/19 19:35:52 by ehafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include <string>
#include <sstream>
#include <list>
#include <cstdlib>
#include <iostream>
#include <unistd.h> // pour utiliser sleep() -> debug, a virer plus tard
# define P(x) std::cout << x << std::endl;  //just for print easily simple things (thx Artainmo)
# define PP(x) std::cout << "|" << x << "|" << std::endl;
/*
** TODO :
** - Insensible à la casse pour les noms de champs
** - regarder comment faire pour les champs separes etc
** - voir pour les whitespaces avant la methode
*/
/*
** Function for tests
** Print the content of a container
*/
template <class Container>
void	print_list(Container l)
{
	typename Container::iterator it = l.begin();
	int i = 0;
	P("-- print container --");
	while (it != l.end())
	{
		std::cout << "Elem " << i++ << " : |" << *it << "|" << std::endl;
		++it;
	}
};
/*
** Print all elements in header struct
*/
void	print_header(t_header *h)
{
	P("------------------- START HEADER -------------------");
	P("-- Accept_Charset : ");
	if (!h->Accept_Charsets.empty())
	{
		print_list(h->Accept_Charsets);
	}
	else
		P("Empty");
	P("-- Accept_Language : ");
	if (!h->Accept_Language.empty())
	{
		print_list(h->Accept_Language);
	}
	else
		P("Empty");
	P("-- Allow : ");
	if (!h->Allow.empty())
	{
		print_list(h->Allow);
	}
	else
		P("Empty");
	P("-- Authorization : ");
	if (!h->Authorization.empty())
	{
		print_list(h->Authorization);
	}
	else
		P("Empty");
	P("-- Content_Language : ");
	if (!h->Content_Language.empty())
	{
		print_list(h->Content_Language);
	}
	else
		P("Empty");
	P("-- Content_Length : ");
	if (!h->Content_Length.empty())
	{
		print_list(h->Content_Length);
	}
	else
		P("Empty");
	P("-- Content_Location : ");
	if (!h->Content_Location.empty())
	{
		print_list(h->Content_Location);
	}
	else
		P("Empty");
	P("-- Content_Type : ");
	if (!h->Content_Type.empty())
	{
		print_list(h->Content_Type);
	}
	else
		P("Empty");
	P("-- Date : ");
	if (!h->Date.empty())
	{
		print_list(h->Date);
	}
	else
		P("Empty");
	P("-- Host : ");
	if (!h->Host.empty())
	{
		print_list(h->Host);
	}
	else
		P("Empty");
	P("-- Last_modified : ");
	if (!h->Last_modified.empty())
	{
		print_list(h->Last_modified);
	}
	else
		P("Empty");
	P("-- Location : ");
	if (!h->Location.empty())
	{
		print_list(h->Location);
	}
	else
		P("Empty");
	P("-- Referer : ");
	if (!h->Referer.empty())
	{
		print_list(h->Referer);
	}
	else
		P("Empty");
	P("-- retry_after : ");
	if (!h->retry_after.empty())
	{
		print_list(h->retry_after);
	}
	else
		P("Empty");
	P("-- Server : ");
	if (!h->Server.empty())
	{
		print_list(h->Server);
	}
	else
		P("Empty");
	P("-- Transfer_Encoding : ");
	if (!h->Transfer_Encoding.empty())
	{
		print_list(h->Transfer_Encoding);
	}
	else
		P("Empty");
	P("-- User_Agent : ");
	if (!h->User_Agent.empty())
	{
		print_list(h->User_Agent);
	}
	else
		P("Empty");
	P("-- WWW_Authenticate : ");
	if (!h->WWW_Authenticate.empty())
	{
		print_list(h->WWW_Authenticate);
	}
	else
		P("Empty");
	P("------------------- END HEADER -------------------");
}	
/*
** Print all elements in request struct
*/
void	print_req_elem(t_req *req)
{
	P("------------------- REQUEST STRUCT -------------------");
	P("-- Full request : ");
	if (!req->full_req.empty())
	{
		PP(req->full_req);
	}
	else
		P("Empty");
	P("-- Method :");
	if (!req->method.empty())
	{
		PP(req->method);
	}
	else
		P("Empty");
	P("-- URl :");
	if (!req->url.empty())
	{
		PP(req->url);
	}
	else
		P("Empty");
	P("-- Location : ");
	if (!req->location.empty())
	{
		PP("A venir");
	}
	else
		P("Empty");
	P("-- Version : ");
	if (!req->version.empty())
	{
		PP(req->version);
	}
	else
		P("Empty");
	P("-- HEADER :");
	print_header(req->header);
	P("-- body_index : ");
	PP(req->body_index);
	P("------------------- END REQUEST STRUCT -------------------");
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
	return (-1);
}
/*
** True if the start of the line is the name of the field required, false otherwise
** (we can erase the field name here but I don't. Like that we're allowed to use this function later in other cases)
*/
bool	find_field_name(std::string &line, std::string const &name)
{
	if (line.substr(0, name.size()) == name)
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
	return (0);
}
/*
** Parse the header
** find the good header field and split the values into req->header->[header_field]
*/
int		parse_header(t_req *req, std::list<std::string> &lines)
{
	while (!lines.empty())
	{
		if (find_field_name(lines.front(), "Accept-Charsets:"))
			split_fields(req->header->Accept_Charsets, lines.front(), "Accept-Charsets:");
		else if (find_field_name(lines.front(), "Accept-Languages:"))
			split_fields(req->header->Accept_Language, lines.front(), "Accept-Languages:");
		else if (find_field_name(lines.front(), "Allow:"))
			split_fields(req->header->Allow, lines.front(), "Allow:");
		else if (find_field_name(lines.front(), "Authorization:"))
			split_fields(req->header->Authorization, lines.front(), "Authorization:");
		else if (find_field_name(lines.front(), "Content_Language:"))
			split_fields(req->header->Content_Language, lines.front(), "Content_Language:");
		else if (find_field_name(lines.front(), "Content_Length:"))
			split_fields(req->header->Content_Length, lines.front(), "Content_Length:");
		else if (find_field_name(lines.front(), "Content_Location:"))
			split_fields(req->header->Content_Location, lines.front(), "Content_Location:");
		else if (find_field_name(lines.front(), "Content_Type:"))
			split_fields(req->header->Content_Type, lines.front(), "Content_Type:");
		else if (find_field_name(lines.front(), "Date:"))
			split_fields(req->header->Date, lines.front(), "Date:");
		else if (find_field_name(lines.front(), "Host:"))
			split_fields(req->header->Host, lines.front(), "Host:");
		else if (find_field_name(lines.front(), "Last_modified:"))
			split_fields(req->header->Last_modified, lines.front(), "Last_modified:");
		else if (find_field_name(lines.front(), "Referer:"))
			split_fields(req->header->Referer, lines.front(), "Referer:");
		else if (find_field_name(lines.front(), "retry_after:"))
			split_fields(req->header->retry_after, lines.front(), "retry_after:");
		else if (find_field_name(lines.front(), "Server:"))
			split_fields(req->header->Server, lines.front(), "Server:");
		else if (find_field_name(lines.front(), "Transfer_Encoding:"))
			split_fields(req->header->Transfer_Encoding, lines.front(), "Transfer_Encoding:");
		else if (find_field_name(lines.front(), "User_Agent:"))
			split_fields(req->header->User_Agent, lines.front(), "User_Agent:");
		else if (find_field_name(lines.front(), "WWW_Authenticate:"))
			split_fields(req->header->WWW_Authenticate, lines.front(), "WWW_Authenticate:");
		else
			std::cout << "Not implemented" << std::endl; // A voir comment gérér les autres cas
		lines.pop_front();
	}
	return (0);
}
/*
** Take the first line of the request split her in a list, 
** add values in the struct t_req and delete used content to the lines list
** return negative value in case of error
*/
int		parse_first_line(t_req *req, std::list<std::string> &lines)
{
	std::string 			line(lines.front());
	std::list<std::string>	split;
	if (line.find("HTTP/1.1", 0) == std::string::npos)
		return (-1);
	split = split_in_list(line, " ");
	req->method = split.front();
	split.pop_front();
	req->url = split.front();
	split.pop_front();
	req->version = split.front();
	lines.pop_front();
	return (0);
}
/*
** Initialise struct t_req
*/
void	init(t_req *req)
{
	req->body_index = 0;
}
/*
** main function for the parsing of the request
** for the moment return a negative number in case of weird behavior (!!) modifier retours erreurs etc
*/
int		parse_request(t_req *req)
{
	std::list<std::string> list_lines;
	init(req);
	if ((req->body_index = get_body_index(req->full_req)) == -1)
		return (-1);
	list_lines = split_in_list(req->full_req.substr(0, req->body_index), "\t\n\r\v\f");
	print_list(list_lines); //test
	parse_first_line(req, list_lines);
	parse_header(req, list_lines);
	print_req_elem(req); //test
	return (0);
}
/* \r\n\r\nvariable1=valeur\r\n */
/*int main()
{
	t_req		r;
	t_header	h;
	r.full_req = "GET / HTTP/1.1\r\nHost: www.site.com\r\nAccept-Charsets: utf-8\r\nAccept: text/html\r\n\r\nvariable1=valeur\r\n";
	r.header = &h;
	parse_request(&r);
	return (0);
}*/