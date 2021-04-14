#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>



int main(int ac, char **av)
{
    if (ac < 2)
    {
        std::cout << "ERROR : file config needed"<< std::endl;
        exit(1);
    }
    // 1 : parsing de la config
    // 2 : setup server


    return 0;
}
