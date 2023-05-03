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
    std::string b("1 ...");
    std::string a("123");
    boost::thread thr_4 = server_connection.thread_send_data(a, b);
    boost::thread thr = server_connection.thread_send_data(a, b);
    boost::thread r_thr_1 = server_connection.thread_read_data();
    boost::thread thr_1 = server_connection.thread_send_data(a, b);
    boost::thread r_thr_2 = server_connection.thread_read_data();
    boost::thread thr_2 = server_connection.thread_send_data(a, b);
    thr.join();
    thr_1.join();
    thr_2.join();
    thr_4.join();
    r_thr_1.join();
    r_thr_2.join();

    std::cout<<server_connection.read_data_array.size()<<std::endl;
    for(int i = 0; i < server_connection.read_data_array.size(); i++)
    {
        std::cout<<"name: "<<server_connection.read_data_array[i].data_name()<<std::endl<<"buffer: ";
        for(auto i : (*server_connection.read_data_array[i].data_str_ptr()))
        {
            std::cout<<i;
        }
        std::cout<<std::endl;
    }
    
    server_connection.close_connection();
    std::cout<<"client"<<std::endl;
}