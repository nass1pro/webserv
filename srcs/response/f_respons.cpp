/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_respons.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stuntman <stuntman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/27 14:02:05 by nahaddac          #+#    #+#             */
/*   Updated: 2021/07/13 10:26:27 by stuntman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"
#include "../../include/utils.hpp"
#include "../../include/cgi.hpp"
#include "../../include/config.hpp"

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
	req.header.Content_Type = std::string("Content-Type: text/html; charset=UTF-8");
}

void setDate(t_req &req)
{
	std::ostringstream ss;
	std::time_t t = std::time(0);
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

void setLastModified(t_req &req)
{
	req.header.Last_modified = std::string("Last-Modified: ");
	req.header.Last_modified.append(req.header.Date.substr(6));
}

void setLocation(t_req &req, int statusCode)
{
	if (statusCode == 201)
	{
		req.header.Location = std::string("Location: ");
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
	if (statusCode == 400)
		req.header.WWW_Authenticate = std::string("WWW-Authenticate: something");
	else
		req.header.WWW_Authenticate = std::string("\0");
}

void setContentLocation(t_req &req, int statusCode)
{
    if (statusCode == 200 || statusCode == 201)
	{
		req.header.Content_Location = std::string("Content-Location: ");
        req.header.Content_Location.append(req.url);
	}
	else if (statusCode == 405 || statusCode == 400|| statusCode == 404 || statusCode == 413 || statusCode == 500)
	{
		req.header.Content_Location = std::string("Content-Location: ");
		req.header.Content_Location.append(req.error_path);
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
	if ((req.method == "POST" || req.method == "PUT") && (statusCode == 200 || statusCode == 201))
	{
		setLastModified(req);
	}
	else
		req.header.Last_modified = std::string("Last-Modified: Sat, 21 Apr 2021 15:19:42 GMT");
	setLocation(req, statusCode);
	setRetryAfter(req, statusCode);
	setServer(config, req);
	setTransferEncoding(req);
	setWWWAuthenticate(req, statusCode);
	res.statusCode = statusCode;


}

void concatenate_header( t_res &res, t_req &req)
{
	if (req.header.Content_Length != "\0")
	{
		if (res.statusCode == 200)
			res.response_header.append("HTTP/1.1 200 OK");
		else if (res.statusCode == 201)
			res.response_header.append("HTTP/1.1 201 Created");	
		else if (res.statusCode == 405)
		{
			res.response_header.append("HTTP/1.1 405 ");
			res.response_header.append("\r\n");
			res.response_header.append(req.header.Allow);
		}
		else if (res.statusCode == 400)
			res.response_header.append("HTTP/1.1 400 ");
		else if (res.statusCode == 404)
			res.response_header.append("HTTP/1.1 404 Not Found");
		else if (res.statusCode == 413)
			res.response_header.append("HTTP/1.1 413 ");
		else if (res.statusCode == 500)
			res.response_header.append("HTTP/1.1 500 ");            
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

void error_500(t_res &res, t_config &config, t_req &req)
{
    std::ifstream	ifs;
    ifs.open(config.err_500.c_str());
    res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    set_response_data(res, config, req, 500);
}

void request_get(t_res &res, t_config &config, t_req &req)
{
    std::ifstream ifs(req.url.c_str());

    if (req.location.cgi.active)
    {
	    if ((req.url = start_cgi(req, config)) == "None")
        {
            error_500(res, config, req);
            ifs.close();
            return;
        }
    }
	res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    set_response_data(res, config, req, 200);
	ifs.close();
}

void request_heads(t_res &res, t_config &config, t_req &req)
{
    std::ifstream ifs(req.url.c_str());
	res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	set_response_data(res, config, req, 200);
	ifs.close();
}

void request_post(t_res &res, t_config &config, t_req &req)
{

    if (req.location.cgi.active)
    {
	    if ((req.url = start_cgi(req, config)) == "None")
        {
            error_500(res, config, req);
            return;
        }
		res.payload.append(req.body_content);
		if (is_exist(req.url))
		{
			set_response_data(res, config, req, 200);
		}
		else
			set_response_data(res, config, req, 201);
		return ;
	}
	if (is_exist(req.url))
	{
		set_response_data(res, config, req, 200);
		std::ofstream newfile(req.url.c_str());
		newfile << req.body_content;
		newfile.close();
	}
	else 
	{
		set_response_data(res, config, req, 201);
		std::ofstream newfile(req.url.c_str());
		newfile << req.body_content;
		newfile.close();
	}	
}

void request_put(t_res &res, t_config &config, t_req &req)
{
	if (is_exist(req.url))
	{
		set_response_data(res, config, req, 200);
		std::ofstream newfile(req.url.c_str());
		if (!req.body_content.empty())
			newfile << req.body_content;
		newfile.close();
	}
	else 
	{
		set_response_data(res, config, req, 201);
		std::ofstream newfile(req.url.c_str());
		if (!req.body_content.empty())
			newfile << req.body_content;
		newfile.close();
	}	
}

void erras_req_client(std::map<int, t_req>::iterator &client, t_server &server, t_res res)
{
    int cl;
    cl = client->first;
    client++;
	res.response_header.erase();
	res.payload.erase();
    server.req.erase(cl);
}

void function_where_i_receive_request_data_and_return_response( std::map<int, t_req>::iterator &client, t_req &req, t_config &config)
{
    t_res res;

    config.serv.res[client->first].erase();
	config.serv.req[client->first].full_req.clear(); //ajout
    if (req.error != 0)
    {
		if (config.error_page.empty())
		{
			if (req.error == 404)
				req.error_path = config.err_404;//std::string("error_pages/404.html");
			else if (req.error == 405)
				req.error_path = config.err_405; //std::string("error_pages/405.html");
			else if (req.error == 413)
				req.error_path = config.err_413; //std::string("error_pages/413.html");
            else if (req.error == 500)
				req.error_path = config.err_500; //std::string("error_pages/413.html");	    								
		}	
		else
		{
			req.error_path = config.error_page;
		}
		if (req.error == 404)
        {
			std::ifstream	ifs;

			ifs.open(req.error_path.c_str());
	    	res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			set_response_data(res, config, req, 404);
    		concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
     		config.serv.res[client->first].append(res.payload);
		}
        else if (req.error == 405 || req.error == 400)
        {
			req.error = 400;
			std::ifstream	ifs;

			ifs.open(req.error_path.c_str());
			res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
     		set_response_data(res, config, req, 405);
    		concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
			if (req.method != "HEAD")
				config.serv.res[client->first].append(res.payload);		   
        }
        else if (req.error == 413)
        {
			std::ifstream	ifs;

			ifs.open(req.error_path.c_str());
	    	res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			set_response_data(res, config, req, 413);
    		concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
     		config.serv.res[client->first].append(res.payload);
		}
        else if (req.error == 500)
        {
			std::ifstream	ifs;

			ifs.open(req.error_path.c_str());
	    	res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			set_response_data(res, config, req, 500);
    		concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
     		config.serv.res[client->first].append(res.payload);
		}        
	}
    else
    {
		if (req.method == "GET")
        {
            request_get(res, config, req);
            concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
			config.serv.res[client->first].append(res.payload);
		}
        else if (req.method == "HEAD")
        {
            request_heads(res, config, req);
            res.payload = std::string("\0");
            concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
		}
        else if (req.method == "POST")
        {
            request_post(res, config, req);
            concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
    		config.serv.res[client->first].append(res.payload);
        }
        else if (req.method == "PUT")
        {
			request_put(res, config, req);
            concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
			config.serv.res[client->first].append(res.payload);
        }
	}
   	std::cout << "\nRESPONSE HEADER |||||||||||||||||||||||||||\n" << res.response_header << "\n/|||||||||||||||||||| RESPONSE HEADER" << std::endl;

	erras_req_client(client, config.serv, res);

	//std::cout << "Erase_req_client -----------------" << std::endl;
}
