#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "network_connection.h"

void handler(ReadData read_data)
{
    std::cout<<"\nname:\n"<<read_data.data_name()<<"\nbuffer:\n"<<read_data.data_str();
}


void client_session(ClientConnection client_connection)
 {
    std::cout<<"client_session"<<std::endl;
    boost::thread thr = client_connection.thread_cycle_read(handler);
    while (true)
     {
        if(!client_connection.is_socket_open())
        {
            thr.join();
            return;
        }
        try
        {
            std::string b("123 notok");
            std::string a("123");
            client_connection.send_data(a, b);
            client_connection.send_data(a, b);
        }
        catch(...)
        {
            std::cout<<"connection closed"<<std::endl;
            thr.join();
            return;
        }
    }
}

int main()
{
    Server server(2001);
    server.client_waiting(client_session);
}


