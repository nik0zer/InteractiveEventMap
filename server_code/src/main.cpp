#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>


//Here is a simple synchronous server:using boost::asio;

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

void client_session(socket_ptr sock)
 {
    while ( true)
     {
        size_t len;
        std::string data(512, '\0');
        try
        {
            len = sock->receive(boost::asio::buffer(data));
        }
        catch(...)
        {
            std::cout<<"connection closed"<<std::endl;
            return;
        }
        
        if (len > 0) 
        {
            std::cout<<data<<std::endl;
            data.clear();
            try
            {
                sock->write_some(boost::asio::buffer("ok", 2)); 
            }
            catch(...)
            {
                std::cout<<"connection closed"<<std::endl;
                return;
            }
        }
    }
}

int main()
{
    boost::asio::io_service service;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 2001); // listen on 80
    boost::asio::ip::tcp::acceptor acc(service, ep);
    while ( true) 
    {
        socket_ptr sock(new boost::asio::ip::tcp::socket(service));
        acc.accept(*sock);
        boost::thread(client_session, sock);
    }
}


