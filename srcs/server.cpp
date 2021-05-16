/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:31:16 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/16 06:23:53 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/config.hpp"
#include <sys/socket.h>
#include <sys/select.h>



void setup_server(t_conf &conf)
{
    int opt = 1;

    for(int i = 0; i <= WORKER_MAX + 1; i++)
    {
        conf.serv.worker[i] = 0;
    }

    if ((conf.serv.socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "(cannot create socket)";
        exit(1);
    }

    conf.serv.fd_max = conf.serv.server_socket;
	if(setsockopt(conf.serv.server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == -1)
	{
		std::cout << "Error: sotsockopt failed" << std::endl;
		exit(1);
	}

    conf.serv.address.sin_family = AF_INET;
    conf.serv.address.sin_addr.s_addr = inet_addr(c.host.c_str());
    try
    {
        conf.serv.address.sin_port = htons(std::stoi(conf.port.front()));
    }
    catch(std::exception &e)
    {
        std::cout<< "ERROR port is not a number in .conf";
        exit(1);
    }
    if (blind(conf.serv.socket_server, (struct sockaddr *)&conf.serv.address, conf.serv.len_address) == -1)
    {
        std::cout<< "ERROR in blind"<< std::endl;
        exit(1);
    }
    if (listen(conf.serv.socket_server, WORKER_MAX) == -1)
    {
        std::cout<< "ERROR in listen"<< std::endl;
        exit(1);
    }
}

void customer_disconnection(t_server &server, t_active &active)
{
    FD_ZERO(&active.write);
    FD_ZERO(&active.read);

    for (int i = 0; i < server.fd_max; i++)
    {
        if(server.client[i] > 0)
        {
            clien_disconnection(server, i, true);
        }
    }
}

void customer_restart(t_server &server)
{
    for (int i = 0; i < server.fd_max; i++)
    {
        if(server.client[i] > 0)
        {
            clien_disconnection(server, i, false);
        }
    }

}
void clien_disconnection(t_server &server, unsigned int i, bool disc)
{
    server.respons.erase(server.client[i]);
    server.req.erase(server.client[i]);
    if (disc == true)
    {
        close(server.client[i]);
        server.client[i] = 0;
    }
}

void get_request(t_server s, t_active &active)
{
    int message_len = -1;
    int buff[1000001];

    for(unsigned int i = 0; i < s.fd_max; i++)
    {
        if(FD_ISSET(s.client[i], &active.read))
        {
            if((message_len = recv(s.clien[i], buff, 1000000, 0)) == -1)
            {
                std::cout<<"error"<< std::endl;
                clien_disconnection(server, i, false);
            }
            if(message_len == 0)
            {
                clien_disconnection(server, i, true);
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
