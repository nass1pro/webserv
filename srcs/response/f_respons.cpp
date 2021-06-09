/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_respons.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafidi <ehafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/27 14:02:05 by nahaddac          #+#    #+#             */
/*   Updated: 2021/06/09 13:12:56 by ehafidi          ###   ########.fr       */
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
	if (statusCode == 200 || statusCode == 201 || statusCode == 405 || statusCode == 404)
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
		req.header.Location = std::string("Location: ");
		req.header.Location.append("/frontend/index.html");
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
	else if (statusCode == 405)
	{
		req.header.Content_Location = std::string("Content-Location: ");
		if (req.url == "/")
		{
			req.header.Content_Location.append("/error_pages/405.html");
		}
	}
	else if (statusCode == 404)
	{
		req.header.Content_Location = std::string("Content-Location: ");
		if (req.url == "/")
		{
			req.header.Content_Location.append("/error_pages/404.html");
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

void concatenate_header( t_res &res, t_req &req)
{

	if (req.method == "GET" || req.method == "HEAD" || req.method == "POST" ||req.method == "PUT")
	{
		//std::cout << "CONTENT LENGTH : " << req.header.Content_Length << std::endl;
		if (req.header.Content_Length != "\0")
		{
			// std::cout << " STATUS CODE IN HEADER " << res.statusCode << std::endl;
			if (res.statusCode == 200)
				res.response_header.append("HTTP/1.1 200 OK");
			else if (res.statusCode == 405)
				res.response_header.append("HTTP/1.1 405 ");
			else if (res.statusCode == 404)
				res.response_header.append("HTTP/1.1 404 ");
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
			if (res.statusCode == 200)
				res.response_header.append("\r\n\r\n");
			else
				res.response_header.append("\r\n\r\n");
		}
	}
	// else if (req.method == "PUT" /*|| req.method == "POST"*/)
	// {
	// 	res.response_header.append(req.header.Location);
	// 	res.response_header.append("\n");

	// }
}


void request_get(t_res &res, t_config &config, t_req &req)
{
    std::ifstream ifs(req.url.c_str()); //get the input file stream with the requested url

	// P(req.url);
	if (req.error == 404 )
	{
		// std::cout << " ON pASSE ICI " << req.error << std::endl;
		std::ifstream ifs("error_pages/404.html");
		res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		set_response_data(res, config, req, 404);
	}
    // if (req.location.cgi.active)
    // {
    //     req.url = start_cgi(req, config);
    // }
	else
    {
		res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    	set_response_data(res, config, req, 200);
	}
}

void request_heads(t_res &res, t_config &config, t_req &req)
{
	if (req.url == "frontend/index.html")
	{
		std::ifstream ifs("error_pages/405.html");
		res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		set_response_data(res, config, req, 405);
	}
	else
	{
		std::ifstream ifs(req.url.c_str());
	    res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	    set_response_data(res, config, req, 200);
	}
}

void request_post(t_res &res, t_config &config, t_req &req)
{
	//std::cout << "Content Lengtht : " << req.body_content << std::endl;

    if (req.body_content.size() == 0)
    {
     	// std::cout << " 11111111111111111 " << req.error << std::endl;
	    set_response_data(res, config, req, 405);
        return ;
    }
    if (req.location.cgi.active)
    {
        // std::cout << " 222222222222222222222 " << req.error << std::endl;  
	    req.url = start_cgi(req, config);
		res.payload.append(req.body_content);
		set_response_data(res, config, req, 200);
    }
    if (is_exist(req.location.cgi.SCRIPT_NAME))
    {	
		     	std::cout << " 333333333333333333 " << req.error << std::endl;
		set_response_data(res, config, req, 200);
	}
	// else
    // {
    //         	std::cout << " 4444444444444444444 " << req.error << std::endl;
	//     std::ifstream ifs(req.url.c_str());
    //     set_response_data(res, config, req, 200);
    // }
}

void request_put(t_res &res, t_config &config, t_req &req)
{
	std::cout<< "laaaa"<<std::endl;

    // if (is_exist(req.url) != true)
    // {
		//std::cout<< "laaaa"<<std::endl;
        set_response_data(res, config, req, 200);
		int ind = 0;
		for (std::string::size_type i = req.url.size(); i > 0; i--)
		{
			if (req.url[i] == '/')
				break;
        	ind++;
		}
		//std::cout << " IND " << ind << std::endl;
		//get the correct filename for the file "file_should_exist_after"
		std::string filename = req.url.substr((req.url.size() - --ind) , req.url.size());
		std::ofstream newfile(filename.c_str());
		// addd content of client to new file
		newfile << req.body_content << std::endl;
		newfile.close();
    // }
    // else
    // {
    //     set_response_data(res, config, req, 200);
    //     std::ofstream ifs(req.url.c_str());
    //     ifs << req.body_content << std::endl;
    // }
}

void erras_req_client(std::map<int, t_req>::iterator &client, t_server &server)
{
    int cl;
    cl = client->first;
    client++;

    server.req.erase(cl);
}

// t_res to_determine_method(t_res &res, t_config &config, t_req &req)
// {
//
// }

void function_where_i_receive_request_data_and_return_response( std::map<int, t_req>::iterator &client, t_req &req, t_config &config)
{
    t_res res;
	std::cout << "//////////////////\n/////////////////////\n////////////////////////////\n/////////////////////////\n" << std::endl;
	std::cout<< "FULL REQ|\n" << config.serv.req[client->first].full_req  << "|FULL REQ " <<std::endl;

	// std::cout<< "envoi \n" << config.serv.req[client->first].full_req << "|| \n" <<std::endl;
    // std::cout << "\n-----------------" <<req.error << " error recu \n" <<std::endl;
	std::cout << "CODE : " << req.error << "\n"<< std::endl;
    if (req.error != 0)
    {
        if (req.error == 400)
        {
			std::ifstream ifs("error_pages/404.html");
			res.payload.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
            set_response_data(res, config, req, 400);
    		concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
			// config.serv.res[client->first].append(res.payload);
			// config.serv.res[client->first].append("\r\n\r\n");
        }
        else if (req.error == 404)
        {
            set_response_data(res, config, req, 404);
    		concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
        }
        else if (req.error == 405)
        {
            set_response_data(res, config, req, 405);
    		concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
        }
        else if (req.error == 413)
        {
            set_response_data(res, config, req, 413);
    		concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
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
    		config.serv.res[client->first].append("\r\n\r\n");
        }
        else if (req.method == "HEAD")
        {
            request_heads(res, config, req);
            res.payload = std::string("\0");
            concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
    		config.serv.res[client->first].append(res.payload);
    		// config.serv.res[client->first].append("\r\n\r\n");
        }
        else if (req.method == "POST")
        {
            request_post(res, config, req);
            concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
			std::cout  << " RESPONSEEEEE \n" << config.serv.res[client->first] << std::endl;
    		config.serv.res[client->first].append(res.payload);
			// int fd2 = open("INCHALLAH.txt", O_WRONLY);
			// write(fd2, config.serv.res[client->first].c_str(), config.serv.res[client->first].size());
			// close(fd2);
			
			// std::cout  << " RESPONSEEEEE \n" << config.serv.res[client->first] << std::endl;

			// config.serv.res[client->first].append("\r\n\r\n");
        }
        else if (req.method == "PUT")
        {
            request_put(res, config, req);
            concatenate_header(res, req);
            config.serv.res[client->first].append(res.response_header);
        	config.serv.res[client->first].append(res.payload);
        	// config.serv.res[client->first].append("\r\n\r\n");
        }
		// std::cout  << " RESPONSEEEEE \n" << config.serv.res[client->first] << std::endl;
        // to_determine_method(res, config, req);
    }
	//std::cout  << " RESPONSEEEEE \n" << config.serv.res[client->first] << std::endl;
	// std::cout  << " CONTENT_LENGTH \n" << config.serv.req[client->first].location.cgi.CONTENT_LENGTH << std::endl;
	erras_req_client(client, config.serv);

}