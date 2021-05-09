/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafidi <ehafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 14:21:32 by ehafidi           #+#    #+#             */
/*   Updated: 2021/05/09 04:43:07 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config.hpp"
#include "../../include/parse_cli.hpp"
#include "../../include/server.hpp"

//200 ok 201 created 404 not found 405 method not allowed 413 payload too large 500 internal server error

void setAllow(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
{
	/*This header must be sent if the server responds with a 405
	 Method Not Allowed status code to indicate which request methods can be used.*/
	if (statusCode == 405)
	{
		header.Allow = std::string("Allow: GET, HEAD, PUT, POST");
	}
	else
		header.Allow = std::string("\0");
}

void setContentLanguage(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
{
	/*If no Content-Language is specified,
	the default is that the content is intended for all language audiences.*/
	header.Content_Language = std::string("\0");
}
void setTransferEncoding(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
{
	header.Transfer_Encoding = std::string("\0");
}

void setContentLength(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
{
	//https://tools.ietf.org/html/rfc7230#section-3.3.2
	std::ostringstream ss;
	ss << "Content-Length: ";
	ss << res.payload.size();
	header.Content_Length = ss.str();
}

void setContentType(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
{
	header.Content_Type = std::string("Content-Type: text/html; charset=UTF-8");
}

void setDate(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
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
	header.Date = ss.str();
}

void setLastModified(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
{
	if (statusCode == 200 || statusCode ==  201)
		header.Last_modified = std::string(header.Date);
	else
		header.Last_modified = std::string("\0");
}

void setLocation(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
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
		header.Content_Location = std::string(req.url);
	}
	else
		header.Content_Location = std::string("\0");
}

void setRetryAfter(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
{
	if (statusCode == 413)
		header.retry_after = std::string("Retry-After: 120");
	else
		header.retry_after = std::string("\0");
}

void setServer(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
{
	header.Server = std::string("Server: ");
	header.Server += config.name_server;
	header.Server += ("/1.0");
}

void setWWWAuthenticate(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
{
	if (statusCode == 401)
		header.WWW_Authenticate = std::string("WWW-Authenticate: something");
	else
		header.WWW_Authenticate = std::string("\0");
}

void setPayload(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
{

}

void setContentLocation(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
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
		header.Location = std::string(req.url);
	}
	else
		header.Content_Location = std::string("\0");
}

void set_response_data(t_config &config, t_header &header, t_req &req, t_res &res, int statusCode)
{
	setAllow(config, header, req, res, statusCode);
	setContentLanguage(config, header, req, res, statusCode);
	setContentLength(config, header, req, res, statusCode);
	setContentLocation(config, header, req, res, statusCode);
	setContentType(config, header, req, res, statusCode);
	setDate(config, header, req, res, statusCode);
	setLastModified(config, header, req, res, statusCode);
	setLocation(config, header, req, res, statusCode);
	setRetryAfter(config, header, req, res, statusCode);
	setServer(config, header, req, res, statusCode);
	setTransferEncoding(config, header, req, res, statusCode);
	setWWWAuthenticate(config, header, req, res, statusCode);
}

void head_request(t_config &config, t_header &header, t_req &req, t_res &res)
{
	for (std::list<t_loc>::iterator it = config.locations.begin(); it != config.locations.end(); it++)
	{
		std::string path = config.host + it->location_match;
		if( path == req.url) // means the url exist and the request is valid
		{
			std::ifstream ifs(req.url); //get the input file stream with the requested url
			res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			set_response_data(config, header, req, res, 200);
			return ;
		}
	}
	// if we reach this part of the function means we have a 404 not found, work in progress
	std::ifstream ifs("error_pages/404.html");
	res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	set_response_data(config, header, req, res, 404);
}

void file_create_or_replace(t_config &config, t_header &header, t_req &req, t_res &res)
{
	std::ofstream replace(req.url);
	std::string buffer;
	replace << buffer << "\n";
	replace.close();
}

void put_request(t_config &config, t_header &header, t_req &req, t_res &res)
{
	//response for put method no payload and header very minimalist
	// check if resource exist
	for (std::list<t_loc>::iterator it = config.locations.begin(); it != config.locations.end(); it++)
	{
		std::string potential_file_path = std::string(it->location_match);
		potential_file_path += req.url;
		std::ifstream potential_file(potential_file_path);
		if (potential_file.is_open() == false)
		{
			// if do not exist 201
			res.statusCode = 201;
			res.payload = std::string("\0");
			set_response_data(config, header, req, res, 201);
			file_create_or_replace(config, header, req, res);
			return ;
		}
	}
	// if exist 200
	res.statusCode = 200;
	res.payload = std::string("\0");
	set_response_data(config, header, req, res, 201);
	file_create_or_replace(config, header, req, res);
}

void concatenate_header(t_config &config, t_header &header, t_req &req, t_res &res)
{
	if (req.method == "GET" || req.method == "HEAD")
	{
		if (header.Content_Length != "\0")
		{
			res.response_header = header.Content_Length;
			res.response_header += "\n";
		}
		if (header.Content_Length != "\0")
		{
			res.response_header += header.Content_Location;
			res.response_header += "\n";
		}
		if (header.Content_Length != "\0")
		{
			res.response_header += header.Content_Type;
			res.response_header += "\n";
		}
		if (header.Content_Length != "\0")
		{
			res.response_header += header.Date;
			res.response_header += "\n";
		}
		if (header.Content_Length != "\0")
		{
			res.response_header += header.Last_modified;
			res.response_header += "\n";
		}
		if (header.Content_Length != "\0")
		{
			res.response_header += header.Server;
			res.response_header += "\n";
		}
		if (header.Content_Length != "\0")
		{
			res.response_header += header.Content_Length;
			res.response_header += "\n";
		}
		if (header.Content_Length != "\0")
		{
			res.response_header += header.Location;
			res.response_header += "\n";
		}
	}
	else if (req.method == "PUT" || req.method == "POST")
	{
		res.response_header += header.Content_Location;
		res.response_header += "\n";
	}
}

t_res &function_where_i_receive_request_data_and_return_response(t_config &config, t_header &header, t_req &req)
{
	// bodysize limit si body plus grand, renvoyer erreur
	t_res res;
	if (req.error == 413)
		set_response_data(config, header, req, res, 413);
	else if (req.version != "1.1")
	{
		std::ifstream ifs("error_pages/405.html");
		res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		set_response_data(config, header, req, res, 405);
	}
	else if (req.method == "GET") //read content
		head_request(config, header, req, res);
	else if (req.method == "HEAD") //read header content
	{
		head_request(config, header, req, res);
		res.payload = std::string("\0");
	}
	else if (req.method == "PUT") //update content
		put_request(config, header, req, res);
	else if (req.method == "POST") //create content
		put_request(config, header, req, res);
	else
		set_response_data(config, header, req, res, 405);
	concatenate_header(config, header, req, res);
	return (res);
}
