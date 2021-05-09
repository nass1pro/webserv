#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

#include <ctime>
#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <string.h>
#include <dirent.h>
<<<<<<< HEAD
#include <fstream>

=======
#include <sys/socket.h>
>>>>>>> 0fbfea61e6eea8ce13a515ca8ddfe4ad7ab52bb2

typedef struct s_header
{
    std::string            Accept_Charsets;
    std::list<std::string> Accept_Language;
    std::string            Allow;
    std::list<std::string> Authorization;
<<<<<<< HEAD
    std::string Content_Language;
	std::string Content_Length;
    std::string Content_Location;
    std::string Content_Type;
    std::string Date;
=======
    std::string            Content_Language;
	unsigned int           Content_Length;
    std::string            Content_Location;
    std::string            Content_Type;
    std::string            Date;
>>>>>>> 0fbfea61e6eea8ce13a515ca8ddfe4ad7ab52bb2
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
<<<<<<< HEAD
    std::string body_entire;
=======
    std::string            body_entire;
>>>>>>> 0fbfea61e6eea8ce13a515ca8ddfe4ad7ab52bb2

}                           t_header;

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
}                           t_req;

typedef struct s_res
{
<<<<<<< HEAD
    std::string           response_header;
    unsigned int           statusCode;
	std::string			payload;
} t_res;

t_res &function_where_i_receive_request_data_and_return_response(t_config &config, t_header &header, t_req &req);

//previous req

// typedef struct s_req
// {
//     std::string           req_complet;
//     std::string           method;
//     std::string           url;
//     std::list<std::string> location;
//     std::string           version;

// } t_req;

#endif
=======
    std::string            res_complet;
    unsigned int           statusCode;
	std::string			   payload;
}                          t_res;
>>>>>>> 0fbfea61e6eea8ce13a515ca8ddfe4ad7ab52bb2
