/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 15:43:52 by judecuyp          #+#    #+#             */
/*   Updated: 2021/05/18 09:47:40 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP__
# define __UTILS_HPP__

# include <string>
# include <list>
# include <cstdlib>
# include <iostream>
# include <ctype.h>
# include <unistd.h> // pour utiliser sleep() -> debug, a virer plus tard peut être
# include <fstream>
# include "config.hpp"

/*
** Return codes
*/
# define SUCCESS 0
# define ERROR -1

/*
** Utils defines
*/
# define P(x) std::cout << x << std::endl;  //just for print easily simple things (thx Artainmo)
# define PP(x) std::cout << "|" << x << "|" << std::endl;

/*
** Printers
*/
template <class Container>
void	print_list(Container l);
void	print_header(t_header *h);
void	print_req_elem(t_req *req);
void	print_location(t_loc *loc);
void	print_config(t_config *c);
/*
** Utils functions
*/
bool					is_in_charset(char c, std::string charset);
std::list<std::string>	split_in_list(std::string str, std::string charset);
int						split_fields(std::list<std::string> &fields, std::string &line, std::string const &field_name);
int						split_fields_str(std::string &field, std::string &line, std::string const &field_name);
bool					find_field_name(std::string &line, std::string const &name);

/*
** Request
*/
int		parse_request(t_req *req);

/*
** Config
*/
int		parse_conf(std::string path, std::list<t_config> &conf);
#endif