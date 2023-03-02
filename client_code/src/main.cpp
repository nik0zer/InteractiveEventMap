#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
void client_session(socket_ptr sock)
 {
    while ( true)
     {
        char data[512];
        size_t len = sock->receive(boost::asio::buffer(data));
        if ( len > 0) 
        {
            std::cout<<data<<std::endl;
            sock->write_some(boost::asio::buffer("ok", 2)); 
            sock->close();
            std::cout<<"connection closed"<<std::endl;
            return;
        }
    }
}

int main()
{
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 2001);
    socket_ptr socket(new boost::asio::ip::tcp::socket(io_service));
    socket->connect(endpoint);
    std::string str("hello");
    socket->write_some(boost::asio::buffer(str));
    boost::thread thread(client_session, socket);
    std::cout<<"client"<<std::endl;
    thread.join();
}