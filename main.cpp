#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>

// #define PORT 8080
// void setup_server()
// {
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//     {
//         std::cout << "(cannot create socket)";
//         exit(1);
//     }
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons( PORT );
//     exit(1);
// }


int main(int ac, char **av)
{
    if (ac < 2)
    {
        std::cout << "ERROR : file config needed"<< std::endl;
        exit(1);
    }

    // parsing de la config
    // setupe server
    // lancement du server
        // attente de connection


    setup_server();
    return 0;
}
