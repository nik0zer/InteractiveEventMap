#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>
#include "network_connection.h"

int main()
{
    boost::asio::io_service io_service;
    ServerConnection server_connection(std::string("127.0.0.1"), 2001);
    server_connection.connect_to_server();
    server_connection.send_data("hello");
    server_connection.close_connection();
    std::cout<<"client"<<std::endl;
}