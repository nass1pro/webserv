/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 11:17:38 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/25 13:46:39 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CGI_HPP__
# define __CGI_HPP__

# include "config.hpp"


void        set_env(std::string var, std::string equal_to, std::vector<std::string> &env);
void        set_env_vector(t_cgi const& cgi, std::vector<std::string> &env);
void        set_header_cgi(t_cgi &cgi, t_req &req, t_config &conf,std::vector<std::string> &env);
std::string start_cgi(t_req &req, t_config &conf);


void        init_execve_cgi(t_req const& req, std::vector<std::string> &parameter);
void        parse_cgi_file(t_req &req, std::string const& ouput_file);
bool        fork_cgi(int &fd_upload, t_req &req, std::vector<std::string> const &env);


#endif
