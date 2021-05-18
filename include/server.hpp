/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafidi <ehafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 06:34:03 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/18 10:55:09 by ehafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <ctime>
#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <string.h>
#include <dirent.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <map>
#include "config.hpp"

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
    size_t                  body_size_limit = 1;
	bool					default_server = false;
    t_server                serv;

}                           t_config;

void                        detecte_connection(std::list<t_config> &conf, t_active &active);
void                        write_socket(t_server &server, t_active &active);
void                        read_socket(t_config &conf, t_active &active);
void                        launche_server(std::list<t_config> &conf);


void                        setup_server(t_config &conf);
void                        customer_disconnection(t_server &server, t_active &active);
void                        customer_restart(t_server &server);
void                        client_restart(t_server &server,t_active &active);
void                        clien_disconnection(t_server &server, unsigned int i);
void                        set_socket(t_server &server, t_active &active);
void                        server_init_socket(std::list<t_config> &conf, t_active &active);

void                        get_request(t_server s, t_active &active);
void                        accept_connection(t_server &server);
void                        new_connection(t_server &server, t_actve &active);
void ft_server(std::list<t_config> &conf, t_active &active, void (*f)(t_server &, t_active &));
