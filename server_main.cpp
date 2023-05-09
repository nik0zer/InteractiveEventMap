#include "network_connection_server.h"
#define PORT 2001

// void handler(ReadData read_data, ClientConnection* client_connection_ptr)
// {
//     std::cout<<"\nname:\n"<<read_data.data_name()<<"\nbuffer:\n"<<read_data.data_str()<<std::endl;
//     if(DataBase::get_instance().parse_cmd(read_data.data_name(), read_data.data_str()) == 0)
//     {
//         return;
//     }

//     if(read_data.data_name() == "send_me_actual_persons")
//     {
//         //return std::vector<Event>
//         std::vector<Person> persons = DataBase::get_instance().get_persons_to_sync(std::atol(read_data.data_str().c_str()));
//         for(auto i : persons)
//         {
//             client_connection_ptr->send_data("update_person", i);
//         }
//         client_connection_ptr->send_data("send_me_actual_persons", std::to_string(DataBase::get_instance().get_last_edit_time_persons()));
//     }

//     if(read_data.data_name() == "send_me_actual_events")
//     {
//         //return std::vector<Event>
//         std::vector<Event> events =  DataBase::get_instance().get_events_to_sync(std::atol(read_data.data_str().c_str()));
//         for(auto i : events)
//         {
//             client_connection_ptr->send_data("update_event", i);
//         }
//         client_connection_ptr->send_data("send_me_actual_events", std::to_string(DataBase::get_instance().get_last_edit_time_persons()));
//     }
// }

// void client_session(ClientConnection client_connection)
// {
//     client_connection.thread_cycle_read(handler);
//     while(client_connection.is_socket_open())
//     {

//     }
// }


int main(int argc, char *argv[])
{
    // Server server(PORT);
    // server.client_waiting(client_session);
}