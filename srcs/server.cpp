/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehafidi <ehafidi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:31:16 by nahaddac          #+#    #+#             */
/*   Updated: 2021/07/04 14:40:29 by ehafidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config.hpp"
#include <sys/socket.h>
#include <sys/select.h>
#include "../include/server.hpp"
#include "../include/utils.hpp"


void setup_server(t_config &conf)
{
    int opt = 1;

    conf.serv.len_address = sizeof(conf.serv.address);

    for(int i = 0; i <= CLIENT_MAX + 1; i++)
    {
        conf.serv.client[i] = 0;
    }

    if ((conf.serv.socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "(cannot create socket)";
        exit(1);
    }

    conf.serv.fd_max = conf.serv.socket_server;
	if(setsockopt(conf.serv.socket_server, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == -1)
	{
		std::cout << "Error: sotsockopt failed" << std::endl;
		exit(1);
	}

    conf.serv.address.sin_family = AF_INET;
    conf.serv.address.sin_addr.s_addr = inet_addr(conf.host.c_str());
    try
    {
        conf.serv.address.sin_port = htons(std::atoi(conf.port.front().c_str()));
    }
    catch(std::exception &e)
    {
        std::cout<< "ERROR port is not a number in .conf";
        exit(1);
    }
    if (bind(conf.serv.socket_server, (struct sockaddr *)&conf.serv.address, conf.serv.len_address) == -1)
    {
        std::cout<< "ERROR in bind"<< std::endl;
        exit(1);
    }
    if (listen(conf.serv.socket_server, CLIENT_MAX) == -1)
    {
        std::cout<< "ERROR in listen"<< std::endl;
        exit(1);
    }
}

void clien_disconnection(t_server &server, unsigned int i)
{
    server.res.erase(server.client[i]);
    server.req.erase(server.client[i]);
    close(server.client[i]);
    server.client[i] = 0;
}

void customer_disconnection(t_server &server, t_active &active)
{
    FD_ZERO(&active.write);
    FD_ZERO(&active.read);

    for (unsigned int i = 0; i < server.fd_max; i++)
    {
        if(server.client[i] > 0)
        {
            clien_disconnection(server, i);
        }
    }
}

void customer_restart(t_server &server)
{
    for (unsigned int i = 0; i < server.fd_max; i++)
    {
        if(server.client[i] > 0)
        {
            client_restart(server, i);
        }
    }
}

void    client_restart(t_server &server, unsigned int i)
{
    server.res.erase(server.client[i]);
    server.req.erase(server.client[i]);
}


// void set_socket(t_server &server, t_active &active)
// {
//     FD_SET(server.socket_server, &active.read);
//     FD_SET(server.socket_server, &active.write);

//     for(unsigned int i = 0; i < server.fd_max; i++)
//     {
//         FD_SET(server.client[i], &active.read);
//         if(server.res.find(server.client[i]) != server.res.end())
//         {
//             FD_SET(server.client[i], &active.write);
//         }
//     }
// }

void set_socket(t_server &server, t_active &active)
{
    FD_SET(server.socket_server, &active.read);
    FD_SET(server.socket_server, &active.write);
    for(unsigned int i = 0; i < server.fd_max; i++)
    {
        if (server.client[i] > 0)
        {
            FD_SET(server.client[i], &active.read);
            if(server.res.find(server.client[i]) != server.res.end())
            {
                FD_SET(server.client[i], &active.write);
            }
        }
    }
}

void server_init_socket(std::list<t_config> &conf, t_active &active)
{
    FD_ZERO(&active.read);
    FD_ZERO(&active.write);

    for(std::list<t_config>::iterator i = conf.begin(); i != conf.end(); i++)
    {
        set_socket((*i).serv, active);
    }
}

void get_request(t_server &s, t_active &active)
{
    int message_len = -1;
    char buff[1000001];

    for(unsigned int i = 0; i < s.fd_max; i++)
    {
        if(FD_ISSET(s.client[i], &active.read))
        {
            if((message_len = recv(s.client[i], buff, 1000000, 0)) == -1)
            {
                std::cout<<"error"<< std::endl;
                clien_disconnection(s, i);
            }
            if(message_len == 0)
            {
                clien_disconnection(s, i);
            }

            else
            {
                buff[message_len] = '\0';
                if (s.req[s.client[i]].full_req.size() == 0)
                {
                    s.req[s.client[i]].full_req.reserve(1000000100);
                }
                s.req[s.client[i]].full_req += buff;

            }
        }
    }
}

void accept_connection(t_server &server)
{
    if((server.socket_connection = accept(server.socket_server, (struct sockaddr *)&server.address, (socklen_t *)&server.len_address)) == -1)
    {
        ;
        //error
    }
    if ((unsigned int) server.socket_connection > server.fd_max)
    {
        server.fd_max = server.socket_connection;
    }
    fcntl(server.socket_connection, F_SETFL, /*MSG_NOSIGNAL*/SO_NOSIGPIPE);
    for (unsigned int i = 0; i < server.fd_max; i++)
    {
        if (server.client[i] == 0)
        {
            server.client[i] = server.socket_connection;
            break;
        }
    }
}

void new_connection(t_server &server, t_active &active)
{
    if(FD_ISSET(server.socket_server, &active.read))
    {
        accept_connection(server);
    }
}

void ft_server(std::list<t_config> &conf, t_active &active, void (*f)(t_server &, t_active &))
{
    for (std::list<t_config>::iterator i = conf.begin(); i != conf.end(); i++)
		f((*i).serv, active);
}
