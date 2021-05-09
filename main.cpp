/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:31:29 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/09 06:53:09 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include "include/config.hpp"
#include "include/utils.hpp"
#include "include/server.hpp"

void            detecte_connection(std::list<t_config> &conf, t_active &active)
{
    int error;
    struct timeval timeout = {1, 0};
    if ((error = select(FD_SETSIZE, &active.read, &active.write, &timeout)) == -1)
    {
        std::cout << "Error: select failed" << std::endl;
        detecte_connection( conf, active);
    }

}

void write_socket()
{

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

void            lauche_server(std::list<t_config> &conf)
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
        catch(const std::out_of_range &e)
        {
            P(e.what());
            // relancer le client
        }
        catch(const std::exception &e)
        {
            P(e.what());
            //erreur 500
        }
    }
    catch(std::exception &e)
    {
        P("error server start");
        //relancement du serveur
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

    // parsing de la config  !!!!! parsing_conf(av[1], conf);
    // setupe server boucle pour la conf qui aura plusieur server
    // setup_server(conf) OK;
    // lancement du server // fork chaque server
    // attente de connection

    return 0;
}
