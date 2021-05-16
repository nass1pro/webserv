/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:31:29 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/16 12:05:04 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include "include/config.hpp"
#include "include/utils.hpp"
#include "include/server.hpp"
#include <cstring>


void            detecte_connection(std::list<t_config> &conf, t_active &active)
{
    int error;
    struct timeval timeout = {1, 0};

    server_init_socket(conf, active);
    if ((error = select(FD_SETSIZE, &active.read, &active.write, &timeout)) == -1)
    {
        std::cout << "Error: select failed" << std::endl;
        detecte_connection( conf, active);
    }
    if (error == 0)
    {
        ft_server(conf, active, customer_disconnection);
        detect_connection(conf, active);
    }
    else
    {
        ft_server(conf, active, new_connection);
    }
}

void write_socket(t_server &server, t_active &active)
{
    int message_len;

    for (unsigned int  i = 0; i < server.ft_max; i++)
    {
        if(FD_ISSET(server.client[i], active.write))
        {
            if(message_len = send(server.client[i], server.respons[server.client[i].c_str()], server.respons[server.client[i].size()], MSG_NOSIGNAL) == -1)
            {
                P("ERROR : send failed");
                clien_disconnection(server, i, false);
            }
            else if((size_t)message_len < server.respons[server.client[i].size()])
            {
                server.respons[server.client[i]] = server.respons[server.client[i].substr(message_len, server.respons[server.client[i].size()]);
            }
            else
            {
                server.respons.erase(server.client[i]);
            }
        }
    }
}

void read_socket(t_config &conf, t_active &active)
{
    std::map<int, t_req>::iterator request;

    get_request(conf.serv, active);
    if(conf.serv.req.size() != 0)
    {
        request = conf.serv.req.begin();
        // while(request != conf.serv.req.end())
        // {
        //      parsing request and respons you need t_config
        //    request++;
        // }
    }
}

void            launche_server(std::list<t_config> &conf)
{

    std::list<t_config>::iterator server = conf.begin();
    t_active active;
    try
    {
        while(true)
        {
            detecte_connection(conf, active);
            while(server != conf.end())
            {
                read_socket(*server, active);
                write_socket(*server, active);
                server++;
            }
            server.begin();
        }
        catch (const std::out_of_range &e)
        {
            P(e.what());
            customer_restart((*server).serv);
        }
        catch (const std::exception &e)
        {
            P(e.what());
            //erreur 500
        }
    }
    catch (std::exception &e)
    {
        P("error server start");
        ft_server(conf, active, customer_disconnection);
        launche_server(int &conf);
    }
}

int main(int ac, char **av)
{
    std::list<t_config> conf;

    if (ac < 2)
    {
        std::cout << "ERROR : file config needed"<< std::endl;
        exit(1);
    }
    if (parse_conf(av[1], conf) == -1)
        return 1;
    for(std::list<t_conf>::iterator l = conf.begin(); l != conf.end)
    {
        setup_server(*l);
    }
    launche_server(conf);
    return 0;
}
