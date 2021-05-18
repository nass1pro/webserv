#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# include <ctime>
#include <stdio.h>
# include <iostream>
# include <string.h>
# include <list>
# include <sstream>
# include <string.h>
# include <dirent.h>
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/time.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <vector>
# include <map>
# include <fstream>
# include <sys/select.h>
# include <cstring>
# include <errno.h>
# include <sys/types.h>
# include <stdexcept>
// # include "server.hpp"

# define CLIENT_MAX 200

/*
** CGI meta-variables
*/
typedef struct	s_cgi
{
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
}				t_cgi;

typedef struct s_header
{
	std::string				Accept_Charsets;
	std::list<std::string> 	Accept_Language;
	std::string				Allow;
	std::list<std::string>	Authorization;
	std::string				Content_Language;
	std::string		        Content_Length;
	std::string				Content_Location;
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
	std::list<std::string>	location;
	std::string				version;
	t_header			*header;
	int						body_index;
    int error;
	bool 					done;
	std::string				body_content;
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



}               t_server;


typedef struct s_config
{
    std::list<std::string>  host;
    std::string             name_server;
    std::string             error_page;
    std::list<std::string>  location;
    std::list<std::string>  port;
    std::list<std::string>  index;
    size_t                  body_size_limit = 1;
	bool					default_server = false;
    t_server                serv;

}                           t_config;
typedef struct	s_loc
{
	std::string		location_match;
	std::string		optional_modifier;
	std::string		http_methods;
	size_t			body_size_limit = 1; //MB !!
	std::string		directory_files_search;
	std::string		directory_listing;
	std::string		default_file_directory_request;
	std::string		upload_files_location;
	t_cgi			cgi;
}				t_loc;



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

void setAllow(t_config &config, t_req &req, int statusCode);
void setContentLanguage(t_config &config, t_req &req, int statusCode);
void setTransferEncoding(t_config &config, t_req &req, int statusCode);
void setContentLength( std::map<int, t_req>::iterator &client, t_config &config, t_req &req, int statusCode);
void setContentType(t_config &config, t_req &req, int statusCode);
void setDate( t_config &config, t_req &req, int statusCode);
void setLastModified(t_config &config, t_req &req, int statusCode);
void setLocation(t_config &config, t_req &req, int statusCode);
void setRetryAfter(t_config &config, t_req &req, int statusCode);
void setServer(t_config &config, t_req &req, int statusCode);
void setWWWAuthenticate(t_config &config, t_req &req, int statusCode);
void setPayload(t_config &config, t_req &req, int statusCode);
void setContentLocation(t_config &config, t_req &req, int statusCode);
void set_response_data( std::map<int, t_req>::iterator &client, t_config &config, t_req &req, int statusCode);
void head_request( std::map<int, t_req>::iterator &client, t_config &config, t_req &req);
void file_create_or_replace( std::map<int, t_req>::iterator &client, t_config &config, t_req &req);
void put_request( std::map<int, t_req>::iterator &client, t_config &config, t_req &req);
void concatenate_header( std::map<int, t_req>::iterator &client, t_config &config, t_req &req);
void error_500_handling( std::map<int, t_req>::iterator &client, t_config &config, t_req &req);
void function_where_i_receive_request_data_and_return_response( std::map<int, t_req>::iterator &client, t_req &req, t_config &config);


#endif
