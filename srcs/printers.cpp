/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafidi <ehafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 16:44:41 by judecuyp          #+#    #+#             */
/*   Updated: 2021/05/18 11:09:38 by ehafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config.hpp"
#include "../include/utils.hpp"
#include "../include/server.hpp"

/*
** Function for tests
** Print the content of a container
*/
template <class Container>
void	print_list(Container l)
{
	typename Container::iterator it = l.begin();
	int i = 0;

	P("* print container *");
	while (it != l.end())
	{
		std::cout << "Elem " << i++ << " : |" << *it << "|" << std::endl;
		++it;
	}
};

void	print_member_str(std::string str, std::string label)
{
	//P(label);
	std::cout << label;
	if (!str.empty())
	{
		PP(str);
	}
	else
		P("empty");
}

template <class Container>
void	print_member_list(std::list<Container> list, std::string label)
{
	P(label);
	if (!list.empty())
	{
		print_list(list);
	}
	else
	{
		P("Empty");
	}
}

void	print_location(t_loc *loc)
{
	P("     ## print location ##");
	print_member_str(loc->location_match, "-- Location match : ");
	print_member_str(loc->optional_modifier, "-- Optional modifier : ");
	print_member_str(loc->http_methods, "-- http methods : ");

	//P("-- body_size : ");
	std::cout << "-- body_size : ";
	PP(loc->body_size);

	print_member_str(loc->directory_files_search, "-- directory files search : ");
	print_member_str(loc->directory_listing, "-- directory listing : ");
	print_member_str(loc->default_file_directory_request, "-- default file directory request : ");
	print_member_str(loc->upload_files_location, "-- upload files location : ");

}
/*
** Print all elements in config struct
*/
void	print_config(t_config *c)
{
	P("################### START CONFIG ###################");

	print_member_str(c->host, "-- Host : ");
	print_member_list(c->port, "-- Port : ");
	print_member_str(c->name_server, "-- Server_name : ");
	print_member_list(c->index, "-- Index : ");
	std::cout << "-- Default server : " << c->default_server << std::endl;
	std::cout << "-- Body size : " << c->body_size << std::endl;
	print_member_str(c->error_page, "- Error page : "); //surement modif list

	P("-- Locations : ");
	if (!c->locations.empty())
	{
		std::list<t_loc>::iterator it = c->locations.begin();
		while (it != c->locations.end())
		{
			print_location(&(*it));
			++it;
		}
	}
	else
		P("Empty");

}

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
	P(h->Content_Length);

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

	P("-- body_content :")
	if (!req->body_content.empty())
	{
		P(req->body_content)
	}
	else
		P("No body");

	P("------------------- END REQUEST STRUCT -------------------");
}
