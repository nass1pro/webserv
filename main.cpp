/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:31:29 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/02 01:43:33 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include "include/config.hpp"

void            detecte_connection(std::list<t_config> &conf, t_active &active)
{
    
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

                server++;
            }
        }
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
