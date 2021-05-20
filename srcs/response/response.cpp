/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafidi <ehafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 11:12:57 by ehafidi           #+#    #+#             */
/*   Updated: 2021/05/20 13:42:17 by ehafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"
// #include "../../include/config.hpp"

//200 ok 201 created 404 not found 405 method not allowed 413 payload too large 500 internal server error

void setAllow(t_req &req, int statusCode)
{
	/*This header must be sent if the server responds with a 405
	 Method Not Allowed status code to indicate which request methods can be used.*/
	if (statusCode == 405)
	{
		req.header.Allow = std::string("Allow: GET, HEAD, PUT, POST");
	}
	else
		req.header.Allow = std::string("\0");
}

void setContentLanguage(t_req &req)
{
	/*If no Content-Language is specified,
	the default is that the content is intended for all language audiences.*/
	// std::cout << "SET CONTENT LANGUAGE : " << std::endl;	
	req.header.Content_Language = std::string("\0");
}

void setTransferEncoding(t_req &req)
{
	req.header.Transfer_Encoding = std::string("\0");
}

void setContentLength(t_res &res, t_req &req)
{
	//https://tools.ietf.org/html/rfc7230#section-3.3.2
	std::ostringstream ss;
	ss << "Content-Length: ";
	ss << res.payload.size();
	req.header.Content_Length = ss.str();
}

void setContentType(t_req &req)
{
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
	// std::cout << "name_server : " << config.name_server << std::endl;
	// std::cout << "SATUS CODE : " << req.header.Server << std::endl;

}

void setWWWAuthenticate(t_req &req, int statusCode)
{
	if (statusCode == 401)
		req.header.WWW_Authenticate = std::string("WWW-Authenticate: something");
	else
		req.header.WWW_Authenticate = std::string("\0");
}

// void setPayload(t_config &config, t_req &req, int statusCode)
// {

// }

void setContentLocation(t_req &req, int statusCode)
{
	// std::cout << "SATUS CODE : " << statusCode << std::endl;
	
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
		
		req.header.Content_Location = std::string("Content-Location: ");		
		if (req.url == "/")
		{
			req.header.Content_Location.append("/frontend/index.html");
		}
		// std::cout << "CONTENT LOCATION : " << req.header.Content_Location << std::endl;

	}
	else
	{
		req.header.Content_Location = std::string("\0");
	}
	// std::cout << "CONT LOC : " << req.header.Content_Location <<  std::endl;		

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
	// std::cout << "RES HEADER : " << res.response_header << std::endl;
	// std::cout << "RES Payload : " << res.payload << std::endl;

}

void head_request(t_res &res, t_config &config, t_req &req)
{
	for (std::list<t_loc>::iterator it = config.location.begin(); it != config.location.end(); it++)
	{
		// std::cout << "LOCATION MATCH : " << it->location_match << std::endl;
		// std::cout << "REQ URL : " << req.url << std::endl;
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
				// config.serv.res[client->first].payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
				res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			}

			set_response_data(res, config, req, 200);
			return ;
		}
	}
	// if we reach this part of the function means we have a 404 not found, work in progress
	std::ifstream ifs("error_pages/404.html");
	// config.serv.res[client->first].payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
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
	//response for put method no payload and header very minimalist
	// check if resource exist
	for (std::list<t_loc>::iterator it = config.location.begin(); it != config.location.end(); it++)
	{
		std::string potential_file_path = std::string(it->location_match);
		potential_file_path += req.url;
		std::ifstream potential_file(potential_file_path);
		if (potential_file.is_open() == false)
		{
			// if do not exist 201
			// config.serv.res[client->first].statusCode = 201;
			// config.serv.res[client->first].payload = std::string("\0");
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
			res.response_header.append(req.header.Content_Length);
			res.response_header.append("\n");
		}
		if (req.header.Content_Length != "\0")
		{
			// std::cout << "HERE : " << std::endl;
			// std::cout << "CONT LOC : " << req.header.Content_Location <<  std::endl;		

			res.response_header.append(req.header.Content_Location);
			res.response_header.append("\n");
		}
		if (req.header.Content_Length != "\0")
		{
			res.response_header.append(req.header.Content_Type);
			res.response_header.append("\n");
		}
		if (req.header.Content_Length != "\0")
		{
			res.response_header.append(req.header.Date);
			res.response_header.append("\n");
			// std::cout << "DATE : " << req.header.Date <<  std::endl;		

		}
		if (req.header.Content_Length != "\0")
		{
			res.response_header.append(req.header.Last_modified);
			res.response_header.append("\n");
		}
		if (req.header.Content_Length != "\0")
		{
			res.response_header.append(req.header.Server);
			res.response_header.append("\n");

		}
		// if (req.header.Content_Length != "\0")
		// {
		// 	res.response_header.append(req.header.Content_Length);
		// 	res.response_header.append("\n");

		// }
	}
	else if (req.method == "PUT" || req.method == "POST")
	{
		res.response_header.append(req.header.Location);
		res.response_header.append("\n");

	}
	// std::cout << "RES HEADER : \n" << res.response_header << std::endl;
	// std::cout << "RES HEADER END" << res.response_header << std::endl;
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
	// t_header dns ma t_request
    // bodysize limit si body plus grand, renvoyer erreur
    // function
	// je recois structure de requete dans &client
    // t_response sur lequel je travaille se trouve dans t_server qui se trouve dans t_config
    // t_header dans t_request
	t_res res;
	std::cout << "METHOD : " << req.method << std::endl;
	// std::cout << "ACCEPT CHARSETS : " << req.header->Accept_Charsets << std::endl;
	// std::cout << "ACCEPT LANGUAGE : " << req.header->Accept_Language << std::endl;


    if (req.error == 413)
	{
		std::cout << " /////// 413 ///////// "  << std::endl;
	
		set_response_data(res, config, req, 413);
	}
	else if (req.version != "HTTP/1.1")
	{
		std::cout << " /////// 1.1 ///////// " << req.version << std::endl;
		std::ifstream ifs("error_pages/405.html");
		// config.serv.res[client->first].payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
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
		std::cout << " /////// HEAD ///////// "  << std::endl;
		head_request(res, config, req);
		res.payload = std::string("\0");
	}
	else if (req.method == "PUT") //update content
	{
		std::cout << " /////// PUT ///////// "  << std::endl;
		put_request(res, config, req);
	}
	else if (req.method == "POST") //create content
	{
		std::cout << " /////// POST ///////// "  << std::endl;
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
	// std::cout  << " //////////////ICI " << std::endl;
	
	// int buff = client->first;
	// client++;
	// config.serv.req.erase(buff);
	// std::cout  << << std::endl;

	erras_req_client(client, config.serv);
	// std::cout  << " ////////////// LAAA " << std::endl;
	
	// return (res);
}
