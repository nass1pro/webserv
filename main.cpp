/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:31:29 by nahaddac          #+#    #+#             */
/*   Updated: 2021/06/11 10:39:33 by judecuyp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/config.hpp"
#include "include/utils.hpp"
#include "include/server.hpp"


void            detecte_connection(std::list<t_config> &conf, t_active &active)
{
    int error;
    struct timeval timeout = {1, 0};

    server_init_socket(conf, active);
    if ((error = select(FD_SETSIZE, &active.read, &active.write, NULL,&timeout)) == -1)
    {
        // std::cout << "Error: select failed" << std::endl;
        detecte_connection( conf, active);
    }
    if (error == 0)
    {
        ft_server(conf, active, customer_disconnection);
        detecte_connection(conf, active);
    }
    else
    {
        ft_server(conf, active, new_connection);
    }
}

void write_socket(t_server &server, t_active &active)
{
    int message_len;

    for (unsigned int  i = 0; i < server.fd_max; i++)
    {
        if(FD_ISSET(server.client[i], &active.write))
        {
            if((message_len = send(server.client[i], server.res[server.client[i]].c_str(), server.res[server.client[i]].size(), /*MSG_NOSIGNAL*/SO_NOSIGPIPE)) == -1)
            {
                P("ERROR : send failed");
                clien_disconnection(server, i);
            }
            else if((size_t)message_len < server.res[server.client[i]].size())
            {
                server.res[server.client[i]] = server.res[server.client[i]].substr(message_len, server.res[server.client[i]].size());
            }
            else
            {
                server.res.erase(server.client[i]);
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
        while(request != conf.serv.req.end())
        {
            // std::cout << "---------> " << request->second.full_req << std::endl;
            parse_request( request ,request->second, conf);
            if (request->second.done == true)
            {
                function_where_i_receive_request_data_and_return_response(request, request->second, conf);
            }
           	else
			   request++;
        }
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
            try
            {
                detecte_connection(conf, active);
                while(server != conf.end())
                {
                    read_socket(*server, active);
                    write_socket((*server).serv, active);
                    server++;
                }
                server = conf.begin();
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
    }
    catch (std::exception &e)
    {
        P("error server start");
        ft_server(conf, active, customer_disconnection);
        launche_server(conf);
    }
}

int main(int ac, char **av)
{
    std::list<t_config> conf;
    int ret;

    if (ac < 2)
    {
        std::cout << "ERROR : file config needed"<< std::endl;
        exit(1);
    }
    if ((ret = parse_conf(av[1], conf)))
    {
        if (ret == 2)
            std::cout << "Error: bad config file path." << std::endl;
        else if (ret == ERROR)
            std::cout << "Error: wrong syntax in config file." << std::endl;
        return 1;
    }
	for(std::list<t_config>::iterator l = conf.begin(); l != conf.end(); l++)
    {
        // l->serv.req.resize(200);
        // l->serv.res.resize(200);
        setup_server(*l);
    }
    launche_server(conf);
    return 0;
}
