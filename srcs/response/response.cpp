/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafidi <ehafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 11:12:57 by ehafidi           #+#    #+#             */
/*   Updated: 2021/05/18 12:08:22 by ehafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"

//200 ok 201 created 404 not found 405 method not allowed 413 payload too large 500 internal server error

void setAllow(t_req &req, int statusCode)
{
	/*This header must be sent if the server responds with a 405
	 Method Not Allowed status code to indicate which request methods can be used.*/
	if (statusCode == 405)
	{
		req.header->Allow = std::string("Allow: GET, HEAD, PUT, POST");
	}
	else
		req.header->Allow = std::string("\0");
}

void setContentLanguage(t_req &req)
{
	/*If no Content-Language is specified,
	the default is that the content is intended for all language audiences.*/
	req.header->Content_Language = std::string("\0");
}

void setTransferEncoding(t_req &req)
{
	req.header->Transfer_Encoding = std::string("\0");
}

void setContentLength( std::map<int, t_req>::iterator &client, t_config &config, t_req &req)
{
	//https://tools.ietf.org/html/rfc7230#section-3.3.2
	std::ostringstream ss;
	ss << "Content-Length: ";
	ss << 	config.serv.res[client->first].payload.size();
	req.header->Content_Length = ss.str();
}

void setContentType(t_req &req)
{
	req.header->Content_Type = std::string("Content-Type: text/html; charset=UTF-8");
}

void setDate(t_req &req)
{
	std::ostringstream ss;
	std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
   	ss << "Date: ";
	if (now->tm_wday == 0)
   		ss << "Sun, ";
   	else if (now->tm_wday == 1)
   		ss << "Mon, ";
   	else if (now->tm_wday == 2)
   		ss << "Tue, ";
   	else if (now->tm_wday == 3)
   		ss << "Wed, ";
   	else if (now->tm_wday == 4)
   		ss << "Thu, ";
   	else if (now->tm_wday == 5)
   		ss << "Fri, ";
   	else if (now->tm_wday == 6)
   		ss << "Sat, ";
	ss << now->tm_mday;
   	if (now->tm_mon == 0)
   		ss << " Jan ";
   	else if (now->tm_mon == 1)
   		ss << " Feb ";
   	else if (now->tm_mon == 2)
   		ss << " Mar ";
   	else if (now->tm_mon == 3)
   		ss << " Apr ";
   	else if (now->tm_mon == 4)
   		ss << " May ";
   	else if (now->tm_mon == 5)
   		ss << " Jun ";
   	else if (now->tm_mon == 6)
   		ss << " Jul ";
	else if (now->tm_mon == 7)
   		ss << " Aug ";
   	else if (now->tm_mon == 8)
   		ss << " Sep ";
   	else if (now->tm_mon == 9)
   		ss << " Oct ";
   	else if (now->tm_mon == 10)
   		ss << " Nov ";
   	else if (now->tm_mon == 11)
   		ss << " Dec ";
	ss << (now->tm_year + 1900);
	ss << ' ';
	ss << now->tm_hour;
	ss << ':';
	ss << now->tm_min;
	ss << ':';
	ss << now->tm_sec;
	ss << " GMT";
	req.header->Date = ss.str();
}

void setLastModified(t_req &req, int statusCode)
{
	if (statusCode == 200 || statusCode ==  201)
		req.header->Last_modified = std::string(req.header->Date);
	else
		req.header->Last_modified = std::string("\0");
}

void setLocation(t_req &req, int statusCode)
{
	// used when there is a redirection, "location" is the correct address to return to the client, so he can be redirected.
	if (statusCode == 201)
	{
		// for (std::list<std::string>::iterator it = config.location.begin(); it != config.location.end(); it++)
		// {
		// 	if (*it == "hello") //find condition to find path
		// 	{
		// 		header.Content_Location = std::string(*it);
		// 		break ;
		// 	}
		// }
		req.header->Content_Location = std::string(req.url);
	}
	else
		req.header->Content_Location = std::string("\0");
}

void setRetryAfter(t_req &req, int statusCode)
{
	if (statusCode == 413)
		req.header->retry_after = std::string("Retry-After: 120");
	else
		req.header->retry_after = std::string("\0");
}

void setServer(t_config &config, t_req &req)
{
	req.header->Server = std::string("Server: ");
	req.header->Server += config.name_server;
	req.header->Server += ("/1.0");
}

void setWWWAuthenticate(t_req &req, int statusCode)
{
	if (statusCode == 401)
		req.header->WWW_Authenticate = std::string("WWW-Authenticate: something");
	else
		req.header->WWW_Authenticate = std::string("\0");
}

// void setPayload(t_config &config, t_req &req, int statusCode)
// {

// }

void setContentLocation(t_req &req, int statusCode)
{
	if (statusCode == 200 || statusCode == 201)
	{
		// for (std::list<std::string>::iterator it = config.location.begin(); it != config.location.end(); it++)
		// {
		// 	if (*it == "hello") //find condition to find path
		// 	{
		// 		header.Content_Location = std::string(*it);
		// 		break ;
		// 	}
		// }
		req.header->Location = std::string(req.url);
	}
	else
		req.header->Content_Location = std::string("\0");
}

void set_response_data( std::map<int, t_req>::iterator &client, t_config &config, t_req &req, int statusCode)
{
	setAllow(config, req, statusCode);
	setContentLanguage(config, req, statusCode);
	setContentLength(client, config, req, statusCode);
	setContentLocation(config, req, statusCode);
	setContentType(config, req, statusCode);
	setDate(config, req, statusCode);
	setLastModified(config, req, statusCode);
	setLocation(config, req, statusCode);
	setRetryAfter(config, req, statusCode);
	setServer(config, req, statusCode);
	setTransferEncoding(config, req, statusCode);
	setWWWAuthenticate(config, req, statusCode);
}

void head_request( std::map<int, t_req>::iterator &client, t_config &config, t_req &req)
{
	for (std::list<std::string>::iterator it = config.location.begin(); it != config.location.end(); it++)
	{
		std::string path = *it;
		if(path == req.url) // means the url exist and the request is valid
		{
			std::ifstream ifs(req.url); //get the input file stream with the requested url
			config.serv.res[client->first].payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			set_response_data(client, config, req, 200);
			return ;
		}
	}
	// if we reach this part of the function means we have a 404 not found, work in progress
	std::ifstream ifs("error_pages/404.html");
	config.serv.res[client->first].payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	set_response_data(client, config, req, 404);
}

void file_create_or_replace(t_req &req)
{
	std::ofstream replace(req.url);
	std::string buffer;
	replace << buffer << "\n";
	replace.close();
}

void put_request( std::map<int, t_req>::iterator &client, t_config &config, t_req &req)
{
	//response for put method no payload and header very minimalist
	// check if resource exist
	for (std::list<std::string>::iterator it = config.location.begin(); it != config.location.end(); it++)
	{
		std::string potential_file_path = std::string(*it);
		potential_file_path += req.url;
		std::ifstream potential_file(potential_file_path);
		if (potential_file.is_open() == false)
		{
			// if do not exist 201
			config.serv.res[client->first].statusCode = 201;
			config.serv.res[client->first].payload = std::string("\0");
			set_response_data(client, config, req, 201);
			file_create_or_replace(client, config, req);
			return ;
		}
	}
	// if exist 200
	config.serv.res[client->first].statusCode = 200;
	config.serv.res[client->first].payload = std::string("\0");
	set_response_data(client, config, req, 201);
	file_create_or_replace(client, config, req);
}

void concatenate_header( std::map<int, t_req>::iterator &client, t_config &config, t_req &req)
{
	if (req.method == "GET" || req.method == "HEAD")
	{
		if (req.header->Content_Length != "\0")
		{
			config.serv.res[client->first].response_header.append(req.header->Content_Length);
			config.serv.res[client->first].response_header.append("\n");
		}
		if (req.header->Content_Length != "\0")
		{
			config.serv.res[client->first].response_header.append(req.header->Content_Location);
			config.serv.res[client->first].response_header.append("\n");
		}
		if (req.header->Content_Length != "\0")
		{
			config.serv.res[client->first].response_header.append(req.header->Content_Type);
			config.serv.res[client->first].response_header.append("\n");
		}
		if (req.header->Content_Length != "\0")
		{
			config.serv.res[client->first].response_header.append(req.header->Date);
			config.serv.res[client->first].response_header.append("\n");
		}
		if (req.header->Content_Length != "\0")
		{
			config.serv.res[client->first].response_header.append(req.header->Last_modified);
			config.serv.res[client->first].response_header.append("\n");
		}
		if (req.header->Content_Length != "\0")
		{
			config.serv.res[client->first].response_header.append(req.header->Server);
			config.serv.res[client->first].response_header.append("\n");
		}
		if (req.header->Content_Length != "\0")
		{
			config.serv.res[client->first].response_header.append(req.header->Content_Length);
			config.serv.res[client->first].response_header.append("\n");
		}
		if (req.header->Content_Length != "\0")
		{
			config.serv.res[client->first].response_header.append(req.header->Location);
			config.serv.res[client->first].response_header.append("\n");
		}
	}
	else if (req.method == "PUT" || req.method == "POST")
	{
		config.serv.res[client->first].response_header += req.header->Content_Location;
		config.serv.res[client->first].response_header += "\n";
	}
}

// function for 500 error()
void error_500_handling( std::map<int, t_req>::iterator &client, t_config &config, t_req &req)
{
    config.serv.res[client->first].statusCode = 500;
    std::ifstream ifs("error_pages/500.html");
	config.serv.res[client->first].payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    set_response_data(client, config, req, 500);
    concatenate_header(client, config, req);
}

void function_where_i_receive_request_data_and_return_response( std::map<int, t_req>::iterator &client, t_req &req, t_config &config)
{
	// t_header dns ma t_request
    // bodysize limit si body plus grand, renvoyer erreur
    // function
	// je recois structure de requete dans &client
    // t_response sur lequel je travaille se trouve dans t_server qui se trouve dans t_config
    // t_header dans t_request

    if (req.error == 413)
		set_response_data(client, config, req, 413);
	else if (req.version != "1.1")
	{
		std::ifstream ifs("error_pages/405.html");
		config.serv.res[client->first].payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		set_response_data(client, config, req, 405);
	}
	else if (req.method == "GET") //read content
		head_request(client, config, req);
	else if (req.method == "HEAD") //read header content
	{
		head_request(client, config, req);
		config.serv.res[client->first].payload = std::string("\0");
	}
	else if (req.method == "PUT") //update content
		put_request(client, config, req);
	else if (req.method == "POST") //create content
		put_request(client, config, req);
	else
		set_response_data(client, config, req, 405);
    concatenate_header(client, config, req);
	// return (res);
}
