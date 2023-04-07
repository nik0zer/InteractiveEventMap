#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "network_connection.h"


typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

void client_session(ClientConnection ClientConnection)
 {
    
    while (true)
     {
        std::cout<<"client_session"<<std::endl;
        try
        {
            ClientConnection.read_data();
            std::cout<<"name: "<<ClientConnection.read_data_array[0].data_name()<<"   buffer: "
            <<ClientConnection.read_data_array[0].data_str()<<std::endl;
        }
        catch(...)
        {
            std::cout<<"connection closed"<<std::endl;
            return;
        }

        try
        {
            std::string b("123 notok\n");
            ClientConnection.send_data(ClientConnection.read_data_array[0].data_name() + " " + 
            ClientConnection.read_data_array[0].data_str());
        }
        catch(...)
        {
            std::cout<<"connection closed"<<std::endl;
            return;
        }
    }
}

int main()
{
    Server server(2001);
    server.client_waiting(client_session);
}


