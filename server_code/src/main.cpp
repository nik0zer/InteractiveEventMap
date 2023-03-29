#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "network_connection.h"


typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

void client_session(Client_connection client_connection)
 {
    
    while (true)
     {
        std::cout<<"client_session"<<std::endl;
        try
        {
            client_connection.read_data();
            std::cout<<"name: "<<client_connection.read_data_array[0].data_name()<<"   buffer: "
            <<client_connection.read_data_array[0].data_str()<<std::endl;
        }
        catch(...)
        {
            std::cout<<"connection closed"<<std::endl;
            return;
        }

        try
        {
            std::string b("123 notok\n");
            client_connection.send_data(client_connection.read_data_array[0].data_name() + " " + 
            client_connection.read_data_array[0].data_str());
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


