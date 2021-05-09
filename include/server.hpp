/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 06:34:03 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/09 06:53:46 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <string.h>
#include <dirent.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string>
#include <vector>
#include <map>
#include "include/config.hpp"

# define CLIENT_MAX 200

typedef struct s_active
{
    fd_set read;
    fd_set write;
}               t_active;

typedef struct s_server
{
    struct sockaddr_in          address; //structur pour socket
    int                         socket_server; // socket d'entrer pour le client
    int                         len_address;
    int                         client[CLIENT_MAX + 1];
    int                         socket_connection;
    unsigned int                fd_max;
    std::map<int, t_req>        req;
    std::map<int, std::string>  respons;



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
void                        detecte_connection(std::list<t_config> &conf, t_active &active);
void                        write_socket();
void                        read_socket(t_config &conf, t_active &active);
void                        get_request(t_server s, t_active &active);
