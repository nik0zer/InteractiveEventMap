#include "network_connection.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

Server::Server(int port)
{
    _port  = port;
    _endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port);
}

void Server::client_waiting(void client_session(std::shared_ptr<boost::asio::ip::tcp::socket>))
{
    boost::asio::ip::tcp::acceptor acceptor(_io_service, _endpoint);
    
    while(true)
    {
        std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr(new boost::asio::ip::tcp::socket(_io_service));
        acceptor.accept(*socket_ptr);
        std::shared_ptr<boost::thread> thread_ptr(new boost::thread(client_session, socket_ptr));
        clients.push_back(Client(socket_ptr, thread_ptr));
    }
}