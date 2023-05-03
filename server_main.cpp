#include "network_connection.h"
#include "sql_tools.h"
#define PORT 2001

void handler(ReadData read_data)
{
    std::cout<<"\nname:\n"<<read_data.data_name()<<"\nbuffer:\n"<<read_data.data_str()<<std::endl;
}

void client_session(ClientConnection client_connection)
{
    client_connection.read_data(handler);
}


int main(int argc, char *argv[])
{
    Server server(PORT);
    server.client_waiting(client_session);
}