/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafidi <ehafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 14:21:32 by ehafidi           #+#    #+#             */
/*   Updated: 2021/04/19 20:28:00 by ehafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config.hpp"
#include "../../include/parse_cli.hpp"
#include "../../include/server.hpp"



void setAllow(t_config &config, t_header &header, t_req &req)
{
	/*This header must be sent if the server responds with a 405
	 Method Not Allowed status code to indicate which request methods can be used.*/
	if (req.method != "GET" || req.method != "HEAD" || req.method != "PUT" || req.method != "POST")
	{
		header.Allow = std::string("Allow: GET, HEAD, PUT, POST\n");
	}
	else 
		header.Allow = std::string("\0");
}

void setContentLanguage(t_config &config, t_header &header, t_req &req)
{
	/*If no Content-Language is specified, 
	the default is that the content is intended for all language audiences.*/
	header.Content_Language = std::string("\0");
}
void setTransferEncoding(t_config &config, t_header &header, t_req &req)
{
	header.Transfer_Encoding = "\0";
}

void setContentLength(t_config &config, t_header &header, t_req &req, t_res &res)
{
	//https://tools.ietf.org/html/rfc7230#section-3.3.2

	/*When a message does not have a Transfer-Encoding header field, a
   Content-Length header field can provide the anticipated size, as a
   decimal number of octets, for a potential payload body. */
	if (header.Transfer_Encoding == "\0")
		header.Content_Length = header.body_entire.size();
	else if (req.method == "POST")
		header.Content_Length = header.body_entire.size();		
	else if (res.statusCode == 100 || res.statusCode == 101 || res.statusCode == 204)
		header.Content_Length = NULL;
	else 
		header.Content_Length = NULL;	
}

//200 201 404 405 413 500

//determine which code must be sent and continue headers...

void setContentLocation(t_config &config, t_header &header, t_req &req)
{
 
}

void setContentType(t_config &config, t_header &header, t_req &req)
{
	header.Content_Type = std::string("Content-Type: text/html; charset=UTF-8\n");
}

void setDate(t_config &config, t_header &header, t_req &req)
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
	ss << " GMT \n";	
	header.Date = ss.str();
	// std::cout << header.Date << std::endl;
}

void setLastModified(t_config &config, t_header &header, t_req &req)
{
	header.Last_modified = std::string(header.Date);
}

void setLocation(t_config &config, t_header &header, t_req &req)
{
	for (std::list<std::string>::iterator it = config.location.begin(); it != config.location.end(); it++)
	{
		if (*it == "hello") //find condition to find path
		{
			header.Content_Location = std::string(*it);
			header.Content_Location += "\n";
			break ;
		}
	}
}

void setRetryAfter(t_config &config, t_header &header, t_req &req)
{
	unsigned int statusCode = 200;
	
	if (statusCode == 503 || statusCode == 429 || statusCode == 301)
		header.retry_after = std::string("Retry-After: 120\n"); 
}

void setServer(t_config &config, t_header &header, t_req &req)
{
	// header.Server = std::string("Server: Webserv/1.0\n");
	header.Server = std::string("Server: ");
	header.Server += config.name_server;
	header.Server += ("/1.0\n");	
}

void setWWWAuthenticate(t_config &config, t_header &header, t_req &req)
{	
	//handle it when testing the cgi
	unsigned int statusCode = 200;

	if (statusCode == 401)
		header.WWW_Authenticate = std::string("WWW-Authenticate: something\n"); 
}

void set_statusCode(t_config &config, t_header &header, t_req &req, t_res &res)
{
	/*   If an Accept-Charset header field is present in a request and none of
   the available representations for the response has a charset that is
   listed as acceptable, the origin server can either honor the header
   field, by sending a 406 (Not Acceptable) response, or disregard the
   header field by treating the resource as if it is not subject to
   content negotiation. */ 

   /* If the header field accept-language
   is present in a request and none of the available representations for
   the response have a matching language tag, the origin server can
   either disregard the header field by treating the response as if it
   is not subject to content negotiation or honor the header field by
   sending a 406 (Not Acceptable) response.*/

	// see how nginx behave

	if (req.method != "GET" || req.method != "HEAD" || req.method != "PUT" || req.method != "POST")
	{
		res.statusCode == 405; // Method not allowed
	}
	for (std::list<std::string>::iterator it = config.location.begin(); it != config.location.end(); it++)
	{
		if (*it == req.url) //find condition to find path
		{
			res.statusCode = 200;	
			break ;
		}
	}
		
		
}

void method_not_allowed(t_config &config, t_header &header, t_req &req, t_res &res)
{
	res.statusCode = 405;
	setAllow(config, header, req);
	res.payload = "path of err pages directory";
	res.payload += "/page 405 name"; //at the end payload is the path to the specific error page 
	setDate(config, header, req);
	setServer(config, header, req);
	header.Content_Length = res.payload.size();	
	setContentType(config, header, req);
}

void head_request(t_config &config, t_header &header, t_req &req, t_res &res)
{
	if (req.version != "1.1")
	{
		method_not_allowed(config, header, req, res);
		return ;
	}	
	
	for (std::list<std::string>::iterator it = config.location.begin(); it != config.location.end(); it++)
	{
		if (*it == req.url) // means the url exist and the request is valid
		{
			res.statusCode = 200;
			header.Content_Location = std::string(*it);
			header.Content_Location += "\n";
			setServer(config, header, req);
			header.Content_Length = req.url.size();	
			setContentType(config, header, req);
			setDate(config, header, req);
			setLastModified(config, header, req);
			return ;
		}
	}	
	// if we reach this part of the function means we have a 404 not found
	// it is a template to finish
	res.statusCode = 404;
	header.Content_Location = "path of err pages directory//page 405 name";
	header.Content_Location += "\n";
	setServer(config, header, req);
	header.Content_Length = req.url.size();	
	setContentType(config, header, req);
	setDate(config, header, req);
	setLastModified(config, header, req);	


}

void payload(t_config &config, t_header &header, t_req &req, t_res &res)
{

}

// here I receive all the data parsed by Jules
t_res &function_where_i_receive_request_data(t_config &config, t_header &header, t_req &req)
{
	// bodysize limit si body plus grand, renvoyer erreur
	t_res res;
	//200 ok 201 created 404 not found 405 method not allowed 413 payload too large 500 internal server error

	// set_statusCode(config, header, req, res);
	if (req.method == "GET")
	{
		head_request(config, header, req, res);
		payload(config, header, req, res);
	}
	else if (req.method == "HEAD")	
		head_request(config, header, req, res);		
	else if (req.method == "PUT")	
		//
	else if (req.method == "POST")
		//	
	else 
		method_not_allowed(config, header, req, res);
	
	// if (req.method != "HEAD")
	// {
	// 	setContentLanguage(config, header, req);
	// 	setContentLength(config, header, req);
	// 	setContentType(config, header, req);
	// 	setTransferEncoding(config, header, req);	
	// }
	return (res);
}