#include <ctime>
#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <string.h>
#include <dirent.h>
#include <sys/socket.h>
#include <pthread.h>

# define CLIENT_MAX 200

typedef struct s_active
{
    fd_set read;
    fd_set write;
}               t_active;

typedef struct s_server
{
    struct sockaddr_in  address; //structur pour socket
    int                 socket_server; // socket d'entrer pour le client
    int                 len_address;
    int                 client[CLIENT_MAX + 1];
    int                 socket_connection;
    unsigned int        fd_max;

}               t_server;


typedef struct  s_config
{
    std::list<std::string>  host;
    std::string             name_server;
    std::string             error_page;
    std::list<std::string>  location;
    std::list<std::string>  port;
    std::list<std::string>  index;
    size_t                  body_size_limit;
    t_server                serv;

}                           t_config;

void                        setup_server(t_config &conf);
