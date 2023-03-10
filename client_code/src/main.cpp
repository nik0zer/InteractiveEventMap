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
    boost::thread thr_2(&ServerConnection::send_data<std::string>, &server_connection, b);
    boost::thread thr_3(&ServerConnection::send_data<std::string>, &server_connection, b);
    boost::thread thr_1(&ServerConnection::send_data<int>, &server_connection, 5);

    //dont safe to use thread in send if send often several sent data can be shuffle in server buffer
    //ToDo: write system to protect from this or dont use tread for this

    thr.join();
    thr_1.join();
    thr_2.join();
    thr_3.join();
    server_connection.close_connection();
    std::cout<<"client"<<std::endl;
}