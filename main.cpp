#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include "include/config.hpp"



/* void         routine_thread(std::list<t_config> conf, t_active socket_client)
*/ 

void            lauche_server(std::list<t_config> &conf)
{

    std::list<t_config>::iterator server = conf.begin();
    try
    {
        while(true)
        {
            create_thread(conf, ) /* attente de connection pour chaque
                                     thread ->  
                                  */
        }
    }
}

int main(int ac, char **av)
{
    std::list<t_config> conf;

    if (ac < 2)
    {
        std::cout << "ERROR : file config needed"<< std::endl;
        exit(1);
    }

    // parsing de la config  !!!!! parsing_conf(av[1], conf);
    // setupe server boucle pour la conf qui aura plusieur server
    // setup_server(conf) OK;
    // lancement du server // fork chaque server
    // attente de connection
    
    return 0;
}
