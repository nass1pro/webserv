#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include "include/config.hpp"




void            lauche_server(std::list<t_config> &conf)
{
    std::list<t_config>::iterator server = conf.begin();
    /*
        boucle infinie qui va attendre des connection
        erreur a relever...
    */
}

int main(int ac, char **av)
{
    std::list<t_config> conf;

    if (ac < 2)
    {
        std::cout << "ERROR : file config needed"<< std::endl;
        exit(1);
    }

    //parsing de la config  !!!!! parsing_conf(av[1], conf);

    // setupe server boucle pour la conf qui aura plusieur server
    // setup_server(conf) OK;

    // lancement du server
    // attente de connection

    setup_server();
    return 0;
}
