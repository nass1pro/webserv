/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 09:56:39 by nahaddac          #+#    #+#             */
/*   Updated: 2021/06/01 12:14:50 by judecuyp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cgi.hpp"
#include "../include/utils.hpp"



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
	set_env("PATH_INFO", cgi.PATH_INFO, env);
	set_env("PATH_TRANSLATED", cgi.PATH_TRANSLATED, env);
	set_env("QUERY_STRING", cgi.QUERY_STRING, env);
	set_env("REMOTE_ADDR", cgi.REMOTE_ADDR, env);
	set_env("REMOTE_INDENT", cgi.REMOTE_IDENT, env);
	set_env("REMOTE_USER", cgi.REMOTE_USER, env);
	set_env("REQUEST_METHOD", cgi.REQUEST_METHOD, env);
	set_env("REQUEST_URI", cgi.REQUEST_URI, env);
	set_env("SCRIPT_NAME", cgi.SCRIPT_NAME, env);
	set_env("SERVER_NAME", cgi.SERVER_NAME, env);
	set_env("SERVER_PORT", cgi.SERVER_PORT, env);
	set_env("SERVER_PROTOCOL", cgi.SERVER_PROTOCOL, env);
	set_env("SERVER_SOFTWARE", cgi.SERVER_SOFTWARE, env);

    //secret env ???????
}

void set_header_cgi(t_cgi &cgi, t_req &req, t_config &conf,std::vector<std::string> &env)
{
    cgi.CONTENT_LENGTH  = ft_itoa((int)req.body_content.size());/*std::to_string(req.body_content.size())*/;
    cgi.CONTENT_TYPE    = req.header.Content_Type; // A mettre en liste
    cgi.PATH_INFO       = req.url;
    cgi.PATH_TRANSLATED = req.url;
   //cgi.REMOTE_ADDR     = std::to_string(conf.serv.address.sin_addr.s_addr);
    cgi.REMOTE_USER     = req.header.Host.front();
    cgi.REQUEST_METHOD  = req.method;
    cgi.REQUEST_URI     = req.url;
    cgi.SERVER_NAME     = conf.host;
    cgi.SERVER_PROTOCOL = req.version;

    set_env_vector(cgi, env);

}

void init_execve_cgi(t_req const& req, std::vector<std::string> &parameter)
{
	std::string executable = "/usr/bin/php";

	if (req.location.cgi.SCRIPT_NAME != std::string("None"))
		executable = req.location.cgi.SCRIPT_NAME;
	parameter.push_back(executable);
	parameter.push_back(req.url);
}

void        parse_cgi_file(t_req &req, std::string const &ouput_file)
{
    std::ifstream   fd(ouput_file.c_str());
	std::string     line;
	std::string     new_request;

    if (!fd.is_open())
	{
		fd.close();
		P("Error: parse_cgi_post_file didn't open");
		exit(1);
	}

    getline(fd, line);
    try
	{
		req.error = std::atoi(line.substr(7, 11).c_str());
	}
    catch (std::exception &e)
    {
        fd.close();
		P("Stoi Error: " << e.what());
		// throw server error 500;
    }
    std::string			file((std::istreambuf_iterator<char>(fd)), std::istreambuf_iterator<char>());
	// req.message_body = file.erase(0, find_first_two_line_returns(file) + 1);
	req.body_content = file;
	fd.close();
}

bool        fork_cgi(int &fd_upload, t_req &req, std::vector<std::string> const &env)
{
    pid_t pid;
    int pp[2];

    if (pipe(pp) == -1)
    {
        std::cout<< "ERROR: pipe"<<std::endl;
        //500
    }

    if ((pid = fork()) == -1)
    {
        std::cout<< "ERROR: fork"<<std::endl;
        return false;
    }

    if(!pid)
    {
        close(pp[1]);
        dup2(pp[0], STDIN_FILENO);
        dup2(fd_upload, STDOUT_FILENO);

        std::vector<std::string> parameter;
        init_execve_cgi(req, parameter);
        char *tab_env[env.size() + 1];
        char *tab_execve[parameter.size() + 1];
        for (size_t i = 0; i < env.size(); i++)
        {
            tab_env[i] = (char*)env[i].c_str();
        }
        for (size_t i = 0; i < parameter.size() + 1; i++)
        {
            tab_execve[i] = (char*)parameter[i].c_str();
        }
        tab_env[env.size()] = NULL;
		tab_execve[parameter.size()] = NULL;

        if (execve(tab_execve[0], tab_execve, tab_env) == -1)
		{
			std::cout<<"Error: execve cgi php"<< std::endl;
			///
			close(pp[0]);
			exit(1);
		}
    }
    else
    {
        close(pp[0]);
        write(pp[1], req.body_content.c_str(), req.body_content.size());
        close(pp[1]);
        waitpid(pid, 0, 0);
    }
    return true;
}

std::string start_cgi(t_req &req, t_config &conf)
{
    int fd_upload;
    std::vector<std::string> env;
    std::string ret;

    set_header_cgi(req.location.cgi, req, conf, env);
    ret = req.location.directory_files_search + req.location.upload_files_location;
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
