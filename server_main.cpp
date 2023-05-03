#include "network_connection.h"
#include "sql_tools.h"
#define PORT 2001

void handler(ReadData read_data)
{
    if(DataBase::get_instance().parse_cmd(read_data.data_name(), read_data.data_str()) == 0)
    {
        return;
    }

    if(read_data.data_name() == "send_me_actual_persons")
    {
        //return std::vector<Event>
    }

    if(read_data.data_name() == "send_me_actual_events")
    {
        //return std::vector<Event>
    }

    

}

void client_session(ClientConnection client_connection)
{
    client_connection.thread_cycle_read(handler);
}


int main(int argc, char *argv[])
{
    Server server(PORT);
    server.client_waiting(client_session);
}