#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

class ClientConnection
{

};

class Server
{
    private:
        boost::asio::io_service _io_service;
        boost::asio::ip::tcp::endpoint _endpoint;
        boost::asio::ip::tcp::acceptor _acceptor;
    public:
        std::vector<ClientConnection> clients;

};



#endif