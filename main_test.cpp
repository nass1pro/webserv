/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judecuyp <judecuyp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 13:51:00 by judecuyp          #+#    #+#             */
/*   Updated: 2021/05/13 15:04:36 by judecuyp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/config.hpp"
#include "include/utils.hpp"

int main(int argc, char **argv)
{
    std::string path;
    std::list<t_config> c;

    if (argc > 1)
        path = argv[1];
    else
        path = "config/01.conf";
    parse_conf(path, c);
    std::list<t_config>::iterator it = c.begin();
    while (it != c.end())
    {
        print_config(&(*it));
        ++it;
    }
    return (0);
}