#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

<<<<<<< HEAD
#include <ctime>
#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <string.h>
#include <dirent.h>
#include <fstream>
=======
# include <ctime>
# include <iostream>
# include <string>
# include <list>
# include <sstream>
# include <string.h>
# include <dirent.h>
>>>>>>> 2e07d6f76972ea0068ac76489a9cd59e97988e3d


typedef struct	s_config
{
	std::string				host;
	std::list <std::string>	port;
	std::string				name_server;
	std::string				index;
	std::string				error_page;
	std::list<std::string>	location;

}				t_config;

typedef struct s_header
{
<<<<<<< HEAD
    std::string Accept_Charsets;
    std::list<std::string> Accept_Language;
    std::string Allow;
    std::list<std::string> Authorization;
    std::string Content_Language;
	std::string Content_Length;
    std::string Content_Location;
    std::string Content_Type;
    std::string Date;
    std::list<std::string> Host;
    std::string Last_modified;
    std::list<std::string> Location;
    std::list<std::string> Referer;
    std::string retry_after;
    std::string Server;
    std::string Transfer_Encoding;
    std::list<std::string> User_Agent;
    std::string WWW_Authenticate;
    std::list<std::string> header_entire;
    std::string body_entire;		

} t_header;

typedef struct s_req
{
	std::string             full_req;
	std::string             method;
	std::string             url;
	std::list<std::string>  location;
	std::string             version;
	struct s_header         *header;
	std::string 			body;
	int						body_index;
}              t_req;

typedef struct s_res
{
    std::string           response_header;
    unsigned int           statusCode;
	std::string			payload;
} t_res;

//previous req

// typedef struct s_req
// {
//     std::string           req_complet;
//     std::string           method;
//     std::string           url;
//     std::list<std::string> location;
//     std::string           version;

// } t_req;
=======
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
>>>>>>> 2e07d6f76972ea0068ac76489a9cd59e97988e3d
