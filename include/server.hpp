/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 06:34:03 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/19 14:09:56 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

#include "config.hpp"



void                        detecte_connection(std::list<t_config> &conf, t_active &active);
void                        write_socket(t_server &server, t_active &active);
void                        read_socket(t_config &conf, t_active &active);
void                        launche_server(std::list<t_config> &conf);


void                        setup_server(t_config &conf);
void                        customer_disconnection(t_server &server, t_active &active);
void                        customer_restart(t_server &server);
void                        client_restart(t_server &server,unsigned int i);
void                        clien_disconnection(t_server &server, unsigned int i);
void                        set_socket(t_server &server, t_active &active);
void                        server_init_socket(std::list<t_config> &conf, t_active &active);

void                        get_request(t_server s, t_active &active);
void                        accept_connection(t_server &server);
void                        new_connection(t_server &server, t_active &active);
void ft_server(std::list<t_config> &conf, t_active &active, void (*f)(t_server &, t_active &));

# endif
