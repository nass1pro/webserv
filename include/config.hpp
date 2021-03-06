/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stuntman <stuntman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 09:52:41 by nahaddac          #+#    #+#             */
/*   Updated: 2021/07/13 14:54:33 by stuntman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# include <ctime>
# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <string.h>
# include <list>
# include <sstream>
# include <string.h>
# include <dirent.h>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/time.h>
# include <fcntl.h>
# include <vector>
# include <map>
# include <fstream>
# include <sys/select.h>
# include <cstring>
# include <errno.h>
# include <sys/types.h>
# include <stdexcept>
# include <fcntl.h>
# include <signal.h>
// # include <csignal>

// # include "server.hpp"

# define CLIENT_MAX 200

/*
** CGI meta-variables
*/
typedef struct	s_cgi
{
	bool		active;
	std::string	AUTH_TYPE;
	std::string	CONTENT_LENGTH;
	std::string	CONTENT_TYPE;
	std::string	GATEWAY_INTERFACE;
	std::string	PATH_INFO;
	std::string	PATH_TRANSLATED;
	std::string	QUERY_STRING;
	std::string	REMOTE_ADDR;
	std::string	REMOTE_IDENT;
	std::string	REMOTE_USER;
	std::string	REQUEST_METHOD;
	std::string	REQUEST_URI;
	std::string	SCRIPT_NAME;
	std::string	SERVER_NAME;
	std::string	SERVER_PORT;
	std::string	SERVER_PROTOCOL;
	std::string	SERVER_SOFTWARE;
	std::string	X_SECRET;
}				t_cgi;

typedef struct	s_loc
{
	std::string				location_match;
	std::string				optional_modifier;
	std::list<std::string>	http_methods;
	size_t					body_size_limit; //MB !!
	std::list<std::string>	index;
	std::string				directory_files_search;
	std::string				directory_listing;
	std::string				default_file_directory_request;
	std::string				upload_files_location;
	t_cgi					cgi;
}				t_loc;

typedef struct s_header
{
	std::string				Accept_Charsets;
	std::list<std::string> 	Accept_Language;
	std::string				Allow;
	std::list<std::string>	Authorization;
	std::string				Content_Language;
	std::string		        Content_Length;
	std::string				Content_Location;
	//content_type = std::list<std::string>
	std::string				Content_Type;
	std::string				Date;
	std::list<std::string>	Host;
	std::string				Last_modified;
	std::string	            Location;
	std::list<std::string>	Referer;
	std::string				retry_after;
	std::string				Server;
	std::string				Transfer_Encoding;
	std::list<std::string>	User_Agent;
	std::string				WWW_Authenticate;
	std::list<std::string>	header_entire;
	std::list<std::string>	Secret_req;
	std::string				body_entire;

} t_header;

typedef struct s_active
{
    fd_set read;
    fd_set write;
}               t_active;

typedef struct	s_req
{
	std::string				full_req;
	std::string				method;
	std::string				url;
	t_loc					location;
	std::string				version;
	t_header				header;
	size_t					body_index;
    int 					error;
	bool 					done;
	std::string				body_content;
	std::string				error_path;
}				t_req;

typedef struct	s_res
{
	std::string		response_header;
	unsigned int	statusCode;
	std::string		payload;
}				t_res;

typedef struct s_server
{
    struct sockaddr_in          address; //structur pour socket
    int                         socket_server; // socket d'entrer pour le client
    int                         len_address;
    int                         client[CLIENT_MAX + 1];
    int                         socket_connection;
    unsigned int                fd_max;
    std::map<int, t_req>        req;
    std::map<int, std::string>  res;
	int pass;
}               t_server;


typedef struct s_config
{
    std::string				host;
	std::string				root;
    std::string             name_server;
    std::string             error_page;
    std::list<t_loc>		location;
    std::list<std::string>  port;
    std::list<std::string>  index;
    size_t                  body_size_limit;
	bool					default_server;
    t_server                serv;

	std::string				err_400;
	std::string				err_404;
	std::string				err_405;
	std::string				err_413;
	std::string				err_500;

}                           t_config;


// typedef struct	s_config
// {
// 	std::string				host;
// 	std::list<std::string>	port;
// 	std::string				name_server;
// 	std::list<std::string>	index;
// 	bool					default_server = false;
// 	std::string				error_page;
// 	std::list<t_loc>		locations;
// 	size_t					body_size_limit = 1; // MB !!

// }				t_config;

void setAllow(t_req &req, int statusCode);
void setContentLanguage(t_req &req);
void setTransferEncoding(t_req &req);
void setContentLength(t_res &res, t_req &req);
void setContentType(t_req &req);
void setDate(t_req &req);
void setLastModified(t_req &req, int statusCode);
void setLocation(t_req &req, int statusCode);
void setRetryAfter(t_req &req, int statusCode);
void setServer(t_config &config, t_req &req);
void setWWWAuthenticate(t_req &req, int statusCode);
// void setPayload(t_config &config, t_req &req, int statusCode);
void setContentLocation(t_req &req, int statusCode);
// void set_response_data( t_res &res, t_req &req, int statusCode);

void request_put( t_res &res, t_config &config, t_req &req);
void request_heads(t_res &res, t_config &config, t_req &req);


void file_create_or_replace(t_req &req);
void concatenate_header( t_res &res, t_req &req);
void error_500_handling(std::map<int, t_req>::iterator &client, t_config &config, t_req &req);
void function_where_i_receive_request_data_and_return_response( std::map<int, t_req>::iterator &client, t_req &req, t_config &config);
void set_response_data( t_res &res, t_config &config, t_req &req, int statusCode);


#endif
