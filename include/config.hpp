
#include <string>
#include <list>

typedef struct s_config
{
    std::string host;
    std::list <std::string> port;
    std::string name_server;
    std::string index;
    std::string error_page;
    std::list<std::string> location;

}              t_config;

typedef struct s_header
{
    std::list<std::string> Accept_Charsets;
    std::list<std::string> Accept_Language;
    std::list<std::string> Allow;
    std::list<std::string> Authorization;
    std::list<std::string> Content_Language;
    std::list<std::string> Content_Length;
    std::list<std::string> Content_Location;
    std::list<std::string> Content_Type;
    std::list<std::string> Date;
    std::list<std::string> Host;
    std::list<std::string> Last_modified;
    std::list<std::string> Location;
    std::list<std::string> Referer;
    std::list<std::string> retry_after;
    std::list<std::string> Server;
    std::list<std::string> Transfer_Encoding;
    std::list<std::string> User_Agent;
    std::list<std::string> WWW_Authenticate;

}

typedef struc s_req
{
    std::string           req_complet;
    std::string           method;
    std::string           url;
    std::list<std::string> location;
    std::string           version;

}
