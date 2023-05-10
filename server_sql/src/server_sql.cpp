#include "server_sql.h"

void client_session(ClientConnection client_connection)
{
    client_connection.cycle_read(handler);
}

void handler(ReadData read_data, ClientConnection* client_connection)
{
    if(read_data.data_name() == "DELETE")
    {
        DataBase_Server::get_instance().remove_event_by_name(((Event)read_data.data_str()).get_name());
        return;
    }
    if(read_data.data_name() == "EVENT")
    {
        DataBase_Server::get_instance().add_event(read_data.data_str());
        return;
    }
    if(read_data.data_name() == "USER")
    {
        DataBase_Server::get_instance().add_user(read_data.data_str());
        return;
    }
    if(read_data.data_name() == "VERIFY")
    {
        if(DataBase_Server::get_instance().verify_user(read_data.data_str()))
        {
            client_connection->send_data("VERIFIED", "");
            std::cout<<"v"<<std::endl;
        }
        else
        {
            client_connection->send_data("NOT_VERIFIED", "");
            std::cout<<"nv"<<std::endl;
        }
        return;
    }
    if(read_data.data_name() == "UPDATE EVENTS")
    {
        std::vector<Event> events = DataBase_Server::get_instance().get_events_to_sync(std::stol(read_data.data_str()));
        for(auto i : events)
        {
            client_connection->send_data("EVENT", i);
        }
        return;
    }
}

