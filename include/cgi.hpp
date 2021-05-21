/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 11:17:38 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/21 11:21:15 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CGI_HPP__
# define __CGI_HPP__

# include "config.hpp"

/* cgi*/
std::string start_cgi(t_req &req, t_config &conf);

/* cgi_fork*/
void        parse_cgi_file(t_req &req, std::string const& ouput_file);
bool        fork_cgi(int &fd_upload, t_req &req, std::vector<std::string> const &env);


#endif
