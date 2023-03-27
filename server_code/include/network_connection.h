#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>



class Client
{
    private:
        std::shared_ptr<boost::asio::ip::tcp::socket> _socket_ptr;
        std::shared_ptr<boost::thread> _client_session_thread_ptr;
    public:
        Client(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr, std::shared_ptr<boost::thread> client_session_thread_ptr) : 
        _socket_ptr(socket_ptr), _client_session_thread_ptr(client_session_thread_ptr){};
};

class Server
{
    private:
        int _port; 
        boost::asio::io_service _io_service;
        boost::asio::ip::tcp::endpoint _endpoint;

    public:
        std::vector<Client> clients;
        Server(int port);
        void client_waiting(void(*client_session)(std::shared_ptr<boost::asio::ip::tcp::socket>));
};



#endif