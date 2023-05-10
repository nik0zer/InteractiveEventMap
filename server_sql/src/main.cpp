#include "server_sql.h"
#define PORT 2001

int main(int argc, char** argv)
{
    int port = PORT;
    if(argc >= 2)
    {
        int flag = sscanf(argv[1], "%d", &port);
        if(flag < 1)
        {
            std::cout<<"invalid port"<<std::endl;
            return -1;
        }
    }
    Server server(port);
    server.client_waiting(client_session);
}