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
    std::string b("jkgdfkg");
    
    boost::thread thr = server_connection.thread_send_data(b);
    boost::thread r_thr_1 = server_connection.thread_read_data();
    thr.join();
    r_thr_1.join();

    std::cout<<server_connection.read_data_array.size()<<std::endl;
    for(int i = 0; i < server_connection.read_data_array.size(); i++)
    {
        std::cout<<"name: "<<server_connection.read_data_array[i]._data_name<<"   buffer: "
        <<*(server_connection.read_data_array[i]._data_str_ptr)<<std::endl;
    }
    
    server_connection.close_connection();
    std::cout<<"client"<<std::endl;
}