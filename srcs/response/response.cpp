/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 17:12:13 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/20 17:12:37 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"
#include "../../include/utils.hpp"
//200 ok 201 created 404 not found 405 method not allowed 413 payload too large 500 internal server error

void setAllow(t_req &req, int statusCode)
{
	if (statusCode == 405)
	{
		req.header.Allow = std::string("Allow: GET, HEAD, PUT, POST");
	}
	else
		req.header.Allow = std::string("\0");
}

void setContentLanguage(t_req &req)
{
	req.header.Content_Language = std::string("\0");
}

void setTransferEncoding(t_req &req)
{
	req.header.Transfer_Encoding = std::string("\0");
}

void setContentLength(t_res &res, t_req &req)
{
	std::ostringstream ss;
	ss << "Content-Length: ";
	ss << res.payload.size();
	req.header.Content_Length = ss.str();
}

void setContentType(t_req &req)
{
	// css php jpg
	req.header.Content_Type = std::string("Content-Type: text/html; charset=UTF-8");
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
	req.header.Date = ss.str();
}

void setLastModified(t_req &req, int statusCode)
{
	if (statusCode == 200 || statusCode ==  201)
	{
		req.header.Last_modified = std::string("Last-Modified: ");

		req.header.Last_modified.append(req.header.Date.substr(6));
	}
	else
		req.header.Last_modified = std::string("\0");
}

void setLocation(t_req &req, int statusCode)
{
	if (statusCode == 201)
	{
		req.header.Location = std::string("Location");
		req.header.Location.append(req.url);
	}
	else
		req.header.Location = std::string("\0");
}

void setRetryAfter(t_req &req, int statusCode)
{
	if (statusCode == 413)
		req.header.retry_after = std::string("Retry-After: 120");
	else
		req.header.retry_after = std::string("\0");
}

void setServer(t_config &config, t_req &req)
{
	req.header.Server = std::string("Server: ");
	req.header.Server.append(config.name_server);
	req.header.Server.append("/1.0");
}

void setWWWAuthenticate(t_req &req, int statusCode)
{
	if (statusCode == 401)
		req.header.WWW_Authenticate = std::string("WWW-Authenticate: something");
	else
		req.header.WWW_Authenticate = std::string("\0");
}

void setContentLocation(t_req &req, int statusCode)
{
	if (statusCode == 200 || statusCode == 201)
	{
		req.header.Content_Location = std::string("Content-Location: ");
		if (req.url == "/")
		{
			req.header.Content_Location.append("/frontend/index.html");
		}
	}
	else
	{
		req.header.Content_Location = std::string("\0");
	}
}

void set_response_data( t_res &res, t_config &config, t_req &req, int statusCode)
{
	setAllow(req, statusCode);
	setContentLanguage( req);
	setContentLength(res, req);
	setContentLocation(req, statusCode);
	setContentType(req);
	setDate(req);
	setLastModified(req, statusCode);
	setLocation(req, statusCode);
	setRetryAfter(req, statusCode);
	setServer(config, req);
	setTransferEncoding(req);
	setWWWAuthenticate(req, statusCode);
	res.statusCode = statusCode;
}

void head_request(t_res &res, t_config &config, t_req &req)
{
	for (std::list<t_loc>::iterator it = config.location.begin(); it != config.location.end(); it++)
	{
		std::string path = it->location_match;
		if(path == req.url) // means the url exist and the request is valid
		{
			if (path == "/")
			{
				std::ifstream ifs("frontend/index.html"); //get the input file stream with the requested url
				// config.serv.res[client->first].payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
				res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
				// std::cout << "RES PAYLOAD : " << res.payload << std::endl;
			}
			else
			{
				std::ifstream ifs(req.url); //get the input file stream with the requested url
				res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			}

			set_response_data(res, config, req, 200);
			return ;
		}
	}
	std::ifstream ifs("error_pages/404.html");
	res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	set_response_data(res, config, req, 404);
}

void file_create_or_replace(t_req &req)
{
	std::ofstream replace(req.url);
	std::string buffer;
	replace << buffer << "\n";
	replace.close();
}

void put_request( t_res &res, t_config &config, t_req &req)
{
	for (std::list<t_loc>::iterator it = config.location.begin(); it != config.location.end(); it++)
	{
		std::string potential_file_path = std::string(it->location_match);
		potential_file_path += req.url;
		std::ifstream potential_file(potential_file_path);
		if (potential_file.is_open() == false)
		{
			res.statusCode = 201;
			res.payload = std::string("\0");

			set_response_data(res, config, req, 201);
			file_create_or_replace(req);

			return ;
		}
	}
	// if exist 200
	set_response_data(res, config, req, 201);
	file_create_or_replace(req);
}

void concatenate_header( t_res &res, t_req &req)
{
	if (req.method == "GET" || req.method == "HEAD")
	{
		if (req.header.Content_Length != "\0")
		{
			res.response_header.append("200");
			res.response_header.append("\r\n");
		}
		if (req.header.Content_Length != "\0")
		{
			res.response_header.append(req.header.Content_Length);
			res.response_header.append("\r\n");
		}
		if (req.header.Content_Length != "\0")
		{
			res.response_header.append(req.header.Content_Location);
			res.response_header.append("\r\n");
		}
		if (req.header.Content_Length != "\0")
		{
			res.response_header.append(req.header.Content_Type);
			res.response_header.append("\r\n");
		}
		if (req.header.Content_Length != "\0")
		{
			res.response_header.append(req.header.Date);
			res.response_header.append("\r\n");
		}
		if (req.header.Content_Length != "\0")
		{
			res.response_header.append(req.header.Last_modified);
			res.response_header.append("\r\n");
		}
		if (req.header.Content_Length != "\0")
		{
			res.response_header.append(req.header.Server);
			res.response_header.append("\r\n\r\n");

		}
	}
	else if (req.method == "PUT" || req.method == "POST")
	{
		res.response_header.append(req.header.Location);
		res.response_header.append("\n");

	}
}

// function for 500 error()
void error_500_handling(std::map<int, t_req>::iterator &client, t_config &config, t_req &req)
{
    t_res res;

	res.statusCode = 500;
    std::ifstream ifs("error_pages/500.html");
	res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    set_response_data(res, config, req, 500);
    concatenate_header(res, req);
	config.serv.res[client->first].append(res.response_header);
	config.serv.res[client->first].append(res.payload);
}

void erras_req_client(std::map<int, t_req>::iterator &client, t_server &server)
{
    int cl;
    cl = client->first;
    client++;
    server.req.erase(cl);
}

void function_where_i_receive_request_data_and_return_response( std::map<int, t_req>::iterator &client, t_req &req, t_config &config)
{
	t_res res;
	std::cout << "METHOD : " << req.method << std::endl;
    if (req.error == 413)
	{
		std::cout << " /////// 413 ///////// "  << std::endl;

		set_response_data(res, config, req, 413);
	}
	else if (req.version != "HTTP/1.1")
	{
		std::cout << " /////// 1.1 ///////// " << req.version << std::endl;
		std::ifstream ifs("error_pages/405.html");
		res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		set_response_data(res, config, req, 405);
	}
	else if (req.method == "GET") //read content
	{
		// std::cout << " /////// GET ///////// "  << std::endl;
		head_request(res, config, req);
	}
	else if (req.method == "HEAD") //read header content
	{
		// std::cout << " /////// HEAD ///////// "  << std::endl;
		head_request(res, config, req);
		res.payload = std::string("\0");
	}
	else if (req.method == "PUT") //update content
	{
		// std::cout << " /////// PUT ///////// "  << std::endl;
		put_request(res, config, req);
	}
	else if (req.method == "POST") //create content
	{
		// std::cout << " /////// POST ///////// "  << std::endl;
		put_request(res, config, req);
	}
	else
	{
		std::cout << " /////// ELSE ///////// "  << std::endl;
		set_response_data(res, config, req, 405);
	}
	concatenate_header(res, req);
	// std::cout  << " RESPONSE HEQDER : \n" << res.response_header << std::endl;
	// res.payload = std::string("\0");
	// std::cout  << " RESPONSE HEQDER END: \n" << std::endl;
	// (void)client;
	//concatenate erthing in corresponding res in map
	config.serv.res[client->first].append(res.response_header);
	// std::cout  << " RESPONSEEEEE \n" << config.serv.res[client->first] << std::endl;

	config.serv.res[client->first].append(res.payload);
	config.serv.res[client->first].append("\r\n\r\n");
	std::cout  << " RESPONSEEEEE \n" << config.serv.res[client->first] << std::endl;

	// std::cout  << " //////////////ICI " << std::endl;

	// int buff = client->first;
	// client++;
	// config.serv.req.erase(buff);
	// std::cout  << << std::endl;

	erras_req_client(client, config.serv);
	// std::cout  << " ////////////// LAAA " << std::endl;

	// return (res);
}
