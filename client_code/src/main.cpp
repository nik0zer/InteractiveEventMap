#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>
#include "network_connection.h"

void handler(ReadData read_data)
{
    std::cout<<"\nname:\n"<<read_data.data_name()<<"\nbuffer:\n"<<read_data.data_str();
}

int main()
{
    ServerConnection connect("127.0.0.1", 2001);
    boost::thread a = connect.thread_read_data(handler);
    connect.thread_send_data("123", "123");
    a.join();
}