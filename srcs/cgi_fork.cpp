/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_fork.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 12:26:36 by nahaddac          #+#    #+#             */
/*   Updated: 2021/05/25 13:53:41 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cgi.hpp"

void init_execve_cgi(t_req const& req, std::vector<std::string> &parameter)
{
	std::string executable = "/usr/bin/php";

	if (req.loc.CGI.SCRIPT_NAME != std::string("None") && file_exists(req.loc.CGI.SCRIPT_NAME))
		executable = req.loc.CGI.SCRIPT_NAME;
	parameter.push_back(executable);
	parameter.push_back(req.URL);
}

void        parse_cgi_file(t_req &req, std::string const &ouput_file)
{
    std::ifstream   fd(ouput_file);
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
		req.error = std::stoi(line.substr(7, 11));
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
        tab_env[env.size()] = nullptr;
		tab_execve[parameter.size()] = nullptr;

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
