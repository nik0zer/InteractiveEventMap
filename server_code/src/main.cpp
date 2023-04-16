#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "network_connection.h"


typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

void client_session(ClientConnection client_connection)
 {
    std::cout<<"client_session"<<std::endl;
    boost::thread thr =  client_connection.thread_cycle_read();
    while (true)
     {
        if(!client_connection.is_socket_open())
        {
            thr.join();
            return;
        }
        try
        {
            std::string b("123 notok\n");
            client_connection.send_data(b);
            client_connection.send_data(b);
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
    for(auto i = server.clients.begin(); i != server.clients.end(); i++)
    {
        i->client_session_ptr->join();
    }
    std::cout<<"end of server code"<<std::endl;
}


