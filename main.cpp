/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:31:29 by nahaddac          #+#    #+#             */
/*   Updated: 2021/07/08 10:34:25 by nahaddac         ###   ########.fr       */
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
        std::cout << "Error: select failed" << std::endl;
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

    for (unsigned int i = 0; i < server.fd_max; i++)
    {
        if(FD_ISSET(server.client[i], &active.write))
        {
            if((message_len = send(server.client[i], server.res[server.client[i]].c_str(), server.res[server.client[i]].size(), MSG_NOSIGNAL/*SO_NOSIGPIPE*/)) == -1)
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

    //std::cout << "JUSTE AVANT DE GET LA REQUEST HEHEEHEHE" << std::endl;
    get_request(conf.serv, active);
    //std::cout << "ON A GETTE LA REQUEST" << std::endl;
    if(conf.serv.req.size() != 0)
    {
        request = conf.serv.req.begin();
        while(request != conf.serv.req.end())
        {
           // std::cout << "JUSTE AVANT LE PARSING DE LA REQQQQ" << std::endl;
            parse_request( request /*,request->second*/, conf);
            //std::cout << "ON A FINI APRES LE PARSING DE LA REQQQQ" << std::endl;
            if (request->second.done == true && !request->second.method.empty())
            {
                //std::cout << "AVANT FONCTION WHERE I RECVEID" << std::endl;
                function_where_i_receive_request_data_and_return_response(request, request->second, conf);
                //std::cout << "ON SORTY DE FUNCTION WHERE I RECEVEID" << std::endl;
            }
           	else
			    request++;
        }
       // std::cout << "ON SORT DE LA COUCLE DE CONF.SERV" << std::endl;
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
                //std::cout << "ON ENTRE DANS TRY ET ON VEUT DETECTER LA CO MA GUEULE" << std::endl;
                detecte_connection(conf, active);
                //std::cout << "ON A DETECTE LA CONNECTON" << std::endl;
                while(server != conf.end())
                {
                    //std::cout << "JUSTE AVANT DE READ SOCKET" << std::endl;
                    read_socket(*server, active);
                   // std::cout << "ON SORT DE READ SOCKETTTT" << std::endl;
    	    	    write_socket((*server).serv, active);
                    //std::cout << "ON A WRITEE LE SOCKETTTT" << std::endl;
                    server++;
                }
                //std::cout << "ON SORT DE LA BOUCLE DE READ SOCKET" << std::endl;
                server = conf.begin();
            }
            catch (const std::out_of_range &e)
            {
                //std::cout << "ON SORT DE READ SOCKETTTT" << std::endl;
                 //std::cout << "ON ENTRE DANS LEXCEPTION OUUUTT OF RAAANGE" << std::endl;
                std::cout << e.what() << std::endl;
                customer_restart((*server).serv);
                server = conf.begin();
            }
            catch (const std::exception &e)
            {
                //std::cout << "ON ENTRE DANS LEXCEPTION WHAATTT" << std::endl;
                std::cout << e.what() << std::endl;
                customer_restart((*server).serv);
                server = conf.begin();
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

void		handler(int sign)
{
	if (sign == SIGINT)
		exit(0);
}

int main(int ac, char **av)
{
    std::list<t_config> conf;
    int ret;
    signal(SIGINT, handler);
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
        else if (ret == 4)
            std::cout << "Bad https_methods." << std::endl;
        exit(ret);
    }
	for(std::list<t_config>::iterator l = conf.begin(); l != conf.end(); l++)
    {
        setup_server(*l);
    }
    launche_server(conf);
    exit(0);
    return 0;
}
