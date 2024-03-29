#include "server_sql.h"

static std::mutex data_base_mutex;

void client_session(ClientConnection client_connection)
{
    client_connection.cycle_read(handler);
}

void handler(ReadData read_data, ClientConnection* client_connection)
{
    std::lock_guard<std::mutex> lock(data_base_mutex);
    if(read_data.data_name() == "UPDATE EVENT")
    {
        Event event = read_data.data_str();
        event.set_time(std::to_string(std::time(NULL)));
        DataBase_Server::get_instance().update_event(event);
        return;
    }
    if(read_data.data_name() == "EVENT")
    {
        Event event = read_data.data_str();
        event.set_time(std::to_string(std::time(NULL)));
        DataBase_Server::get_instance().update_event(read_data.data_str());
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

