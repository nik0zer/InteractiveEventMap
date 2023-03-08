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
    boost::thread thr(&ServerConnection::send_data<std::string>, &server_connection, b);
    std::cout<<"1234567890"<<std::endl;
    server_connection.read_data();
    boost::thread thr_2(&ServerConnection::send_data<std::string>, &server_connection, b);
    boost::thread thr_3(&ServerConnection::send_data<std::string>, &server_connection, b);
    boost::thread thr_1(&ServerConnection::send_data<int>, &server_connection, 5);
    std::cout<<server_connection.read_data_array.size()<<std::endl;
    for(int i = 0; i < server_connection.read_data_array.size(); i++)
    {
        std::cout<<"name: "<<server_connection.read_data_array[i]._data_name/*<<"   buffer: "
        <<server_connection.read_data_array[i]._data_stream_ptr->rdbuf()*/<<std::endl;
    }

    //dont safe to use thread in send if send often several sent data can be shuffle in server buffer
    //ToDo: write system to protect from this or dont use tread for this
    //add system to protect write threads

    thr.join();
    thr_1.join();
    thr_2.join();
    thr_3.join();
    server_connection.close_connection();
    std::cout<<"client"<<std::endl;
}