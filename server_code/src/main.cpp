#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "network_connection.h"


typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

void client_session(Client_connection client_connection)
 {
    std::cout<<"client_session"<<std::endl;
    while ( true)
     {
            try
            {
                std::string b("123 notok\n");
                client_connection.send_data(b);
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
    // boost::asio::io_service service;
    // boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 2001); // listen on 2001
    // boost::asio::ip::tcp::acceptor acc(service, ep);
    // while ( true) 
    // {
    //     socket_ptr sock(new boost::asio::ip::tcp::socket(service));
    //     acc.accept(*sock);
    //     boost::thread(client_session, sock);
    // }

    Server server(2001);
    server.client_waiting(client_session);
}


