#include "client_sql.h"

void handler(ReadData read_data, ServerConnection* server_connection)
{
    std::lock_guard<std::mutex> lock(ClientData::get_instance().verify_mutex);
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
        ClientData::get_instance().data_base.update_event(read_data.data_str());
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
        verify_mutex.lock();
        if(verification != NOT_CHECKED)
        {
            int verify_flag = verification;
            verification = NOT_CHECKED;
            verify_mutex.unlock();
            return verify_flag;
        }
        verify_mutex.unlock();
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

void ClientData::delete_event(Event event)
{
    server_connection_ptr->send_data("DELETE", event);
}

void ClientData::rename_event(Event event, Event event_1)
{
    server_connection_ptr->send_data("DELETE", event);
    server_connection_ptr->send_data("EVENT", event_1);
}