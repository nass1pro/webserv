#include <ctime>
#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <string.h>
#include <dirent.h>
#include <sys/socket.h>

typedef struct s_header
{
    std::string            Accept_Charsets;
    std::list<std::string> Accept_Language;
    std::string            Allow;
    std::list<std::string> Authorization;
    std::string            Content_Language;
	unsigned int           Content_Length;
    std::string            Content_Location;
    std::string            Content_Type;
    std::string            Date;
    std::list<std::string> Host;
    std::string            Last_modified;
    std::list<std::string> Location;
    std::list<std::string> Referer;
    std::string            retry_after;
    std::string            Server;
    std::string            Transfer_Encoding;
    std::list<std::string> User_Agent;
    std::string            WWW_Authenticate;
    std::list<std::string> header_entire;
    std::string            body_entire;

}                           t_header;

typedef struct s_req
{
	std::string             full_req;
	std::string             method;
	std::string             url;
	std::list<std::string>  location;
	std::string             version;
	struct s_header         *header;
	int						body_index;
}                           t_req;

typedef struct s_res
{
    std::string            res_complet;
    unsigned int           statusCode;
	std::string			   payload;
}                          t_res;
