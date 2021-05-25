/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 09:56:39 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/25 16:41:32 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cgi.hpp"



void set_env(std::string var, std::string equal_to, std::vector<std::string> &env)
{
    env.push_back(var + "=" + equal_to);
}

void set_env_vector(t_cgi const& cgi, std::vector<std::string> &env)
{
    set_env("AUTH_TYPE", cgi.AUTH_TYPE, env);
	set_env("CONTENT_LENGTH", cgi.CONTENT_LENGTH, env);
	set_env("CONTENT_TYPE", cgi.CONTENT_TYPE, env);
	set_env("GATEWAY_INTERFACE", cgi.GATEWAY_INTERFACE, env);
	set_env("PATH_INFO", cgi.PATH_INFO, env); //req.URL
	set_env("PATH_TRANSLATED", cgi.PATH_TRANSLATED, env);
	set_env("QUERY_STRING", cgi.QUERY_STRING, env);
	set_env("REMOTE_ADDR", cgi.REMOTE_ADDR, env);
	set_env("REMOTE_INDENT", cgi.REMOTE_IDENT, env);
	set_env("REMOTE_USER", cgi.REMOTE_USER, env);
	set_env("REQUEST_METHOD", cgi.REQUEST_METHOD, env); //env variable -> POST/GET/....
	set_env("REQUEST_URI", cgi.REQUEST_URI, env);
	set_env("SCRIPT_NAME", cgi.SCRIPT_NAME, env);
	set_env("SERVER_NAME", cgi.SERVER_NAME, env);
	set_env("SERVER_PORT", cgi.SERVER_PORT, env);
	set_env("SERVER_PROTOCOL", cgi.SERVER_PROTOCOL, env); //HTTP/1.1
	set_env("SERVER_SOFTWARE", cgi.SERVER_SOFTWARE, env);

    //secret env ???????
}

void set_header_cgi(t_cgi &cgi, t_req &req, t_config &conf,std::vector<std::string> &env)
{
    cgi.CONTENT_LENGTH  = std::to_string(req.body_content.size());
    cgi.CONTENT_TYPE    = req.header.Content_Type.front();
    cgi.PATH_INFO       = req.url;
    cgi.PATH_TRANSLATED = req.url;
    cgi.REMOTE_ADDR     = std::to_string(conf.serv.address.sin_addr.s_addr);
    cgi.REMOTE_USER     = req.header.Host.front();
    cgi.REQUEST_METHOD  = req.method;
    cgi.REQUEST_URI     = req.url;
    cgi.SERVER_NAME     = conf.host;
    cgi.SERVER_PROTOCOL = req.version;

    set_env_vector(cgi, env);

}


std::string start_cgi(t_req &req, t_config &conf)
{
    int fd_upload;
    std::vector<std::string> env;
    std::string ret;

    set_header_cgi(req.location.cgi, req, conf, env);
    ret = req.location.directory_files_search + req.location.upload_files_location;
    // ret = req.location.???? + req.location.upload_files_location;
    if((fd_upload = open(ret.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1)
    {
        close(fd_upload);
        std::cout<<"Error : file upload location error"<<std::endl;
        // trow 500
    }
    if (fork_cgi(fd_upload, req, env) == false)
    {
        close(fd_upload);
        return "None";
    }
    close(fd_upload);
    // if (req.location.cgi.SCRIPT_NAME != std::string("None") && file_exists(req.location.cgi.SCRIPT_NAME))
    if (req.location.cgi.SCRIPT_NAME != std::string("None"))
    {
        parse_cgi_file(req, ret);
    }

    return (ret);
}
