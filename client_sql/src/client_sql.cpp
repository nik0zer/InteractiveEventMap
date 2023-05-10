#include "client_sql.h"

void handler(ReadData read_data, ServerConnection* server_connection)
{
    if(read_data.data_name() == "VERIFIED")
    {
        ClientData::get_instance().verification = VERIFIED;
        return;
    }
    if(read_data.data_name() == "NOT_VERIFIED")
    {
        ClientData::get_instance().verification = NOT_VERIFIED;
        return;
    }
    if(read_data.data_name() == "EVENT")
    {
        ClientData::get_instance().data_base.add_event(read_data.data_str());
        return;
    }
}

void ClientData::set_new_user(User user)
{
    _user = user;
}

void ClientData::registration(User user)
{
    _user = user;
    server_connection_ptr->send_data("USER", _user);
}

void ClientData::add_server_connection_ptr(std::shared_ptr<ServerConnection> server_connection_ptr)
{
    this->server_connection_ptr = server_connection_ptr;
    server_connection_ptr->thread_cycle_read(handler);
}

int ClientData::verify_user()
{
    verification = NOT_CHECKED;
    server_connection_ptr->send_data("VERIFY", _user);
    while(true)
    {
        if(verification != NOT_CHECKED)
        {
            return verification;
        }
    }
}

void ClientData::update_events()
{
    server_connection_ptr->send_data("UPDATE EVENTS", std::to_string(synch_time));
    std::vector<Event> events = data_base.get_events_to_sync(synch_time);
    for(auto i : events)
    {
        server_connection_ptr->send_data("EVENT", i);
    }
}

