/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 09:56:39 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/21 11:20:50 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/cgi.hpp"


std::string start_cgi(t_req &req, t_config &conf)
{
    int fd_upload;
    std::vector<std::string> env;
    std::string ret;

    //init les variable du  cgi

    // ret = req.loc.???? + req.loc.upload_files_location;
    // fd_upload = open(ret.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666)

    //la ou nass fork funct(fd_upload, req, env);
    //close()

    // verifier si les SCRIPT_NAME != none et file existe(SCRIPT_NAME)
        //parse_cgi_file(req, ret);
    return (ret);


}
