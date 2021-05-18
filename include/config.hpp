#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# include <ctime>
# include <iostream>
# include <string>
# include <list>
# include <sstream>
# include <string.h>
# include <dirent.h>

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

typedef struct s_header
{
	std::string				Accept_Charsets;
	std::list<std::string> 	Accept_Language;
	std::string				Allow;
	std::list<std::string>	Authorization;
	std::string				Content_Language;
	unsigned int			Content_Length;
	std::string				Content_Location;
	std::string				Content_Type;
	std::string				Date;
	std::list<std::string>	Host;
	std::string				Last_modified;
	std::list<std::string>	Location;
	std::list<std::string>	Referer;
	std::string				retry_after;
	std::string				Server;
	std::string				Transfer_Encoding;
	std::list<std::string>	User_Agent;
	std::string				WWW_Authenticate;
	std::list<std::string>	header_entire;
	std::string				body_entire;

} t_header;

typedef struct	s_req
{
	std::string				full_req;
	std::string				method;
	std::string				url;
	std::list<std::string>	location;
	std::string				version;
	struct s_header			*header;
	int						body_index;
	std::string				body_content;
}				t_req;

typedef struct	s_res
{
	std::string		res_complet;
	unsigned int	statusCode;
	std::string		payload;
}				t_res;

#endif
