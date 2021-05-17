
#include <ctime>
#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <string.h>
#include <dirent.h>
#include <fstream>
#include <map> /////////////////// to_delete because multiple inclusion
#include <sys/socket.h> /////////////////// to_delete because multiple inclusion

typedef struct	s_loc
{
	std::string		location_match;
	std::string		optional_modifier;
	std::list<std::string>		http_methods; // 1 or more ? 
	size_t			body_size = 1; //MB !!
	std::string		directory_files_search;
	std::string		directory_listing;
	std::string		default_file_directory_request;
	std::string		upload_files_location;

}				t_loc;

typedef struct	s_req
{
	std::string				full_req;
	std::string				method;
	std::string				url;
	std::list<std::string>	location;
	std::string				version;
	struct s_header			*header;
	int						body_index;
	int 					error;
	std::string				body_content;
}				t_req;

typedef struct s_res
{
    std::string           response_header;
    unsigned int           statusCode;
	std::string			payload;
} t_res;

typedef struct s_server
{
    struct sockaddr_in          address; //structur pour socket
    int                         socket_server; // socket d'entrer pour le client
    int                         len_address;
    int                         client[CLIENT_MAX + 1];
    int                         socket_connection;
    unsigned int                fd_max;
    std::map<int, t_req>        req;
    std::map<int, t_res>        res;



}               t_server;


typedef struct	s_config
{
	std::string				name_server;
	std::list<std::string>	index;
	bool					default_server = false;
	std::string				error_page;
	size_t					body_size = 1; // MB !!
    std::list<std::string>  host;
    std::list<t_loc>		locations;
    std::list<std::string>  port;
    size_t                  body_size_limit;
    t_server                serv;

}				t_config;





typedef struct s_header
{
	std::string				Accept_Charsets;
	std::list<std::string> 	Accept_Language;
	std::string				Allow;
	std::list<std::string>	Authorization;
	std::string				Content_Language;
	std::string				Content_Length;
	std::string				Content_Location;
	std::string				Content_Type;
	std::string				Date;
	std::list<std::string>	Host;
	std::string				Last_modified;
	std::string				Location;
	std::list<std::string>	Referer;
	std::string				retry_after;
	std::string				Server;
	std::string				Transfer_Encoding;
	std::list<std::string>	User_Agent;
	std::string				WWW_Authenticate;
	std::list<std::string>	header_entire;
	std::string				body_entire;

} t_header;



// typedef struct	s_res
// {
// 	std::string		res_complet;
// 	unsigned int	statusCode;
// 	std::string		payload;
// }				t_res;

// typedef struct s_config
// {
//     std::string host;
//     std::list <std::string> port;
//     std::string name_server;
//     std::string index;
//     std::string error_page;
//     std::list<std::string> location;

// }              t_config;

// typedef struct s_header
// {
//     std::string Accept_Charsets;
//     std::list<std::string> Accept_Language;
//     std::string Allow;
//     std::list<std::string> Authorization;
//     std::string Content_Language;
// 	std::string Content_Length;
//     std::string Content_Location;
//     std::string Content_Type;
//     std::string Date;
//     std::list<std::string> Host;
//     std::string Last_modified;
//     std::list<std::string> Location;
//     std::list<std::string> Referer;
//     std::string retry_after;
//     std::string Server;
//     std::string Transfer_Encoding;
//     std::list<std::string> User_Agent;
//     std::string WWW_Authenticate;
//     std::list<std::string> header_entire;
//     std::string body_entire;		

// } t_header;

// typedef struct s_req
// {
// 	std::string             full_req;
// 	std::string             method;
// 	std::string             url;
// 	std::list<std::string>  location;
// 	std::string             version;
// 	struct s_header         *header;
// 	std::string 			body;
// 	int						body_index;
// }              t_req;



