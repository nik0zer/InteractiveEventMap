#include "network_connection.h"

ServerConnection::ServerConnection(std::string server_ip, int port)
{
    _server_ip = boost::asio::ip::address::from_string(server_ip);
    _port = port;
    _socket_ptr = boost::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(_io_service));
}

ServerConnection::ServerConnection(boost::asio::ip::address server_ip, int port)
{
    _server_ip = server_ip;
    _port = port;
    _socket_ptr = boost::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(_io_service));
}

void ServerConnection::connect_to_server()
{
    boost::asio::ip::tcp::endpoint endpoint(_server_ip, _port);
    try
    {
        _socket_ptr->connect(endpoint);
    }
    catch(...)
    {
        return;
    }
    _is_connected = true;
}

void ServerConnection::close_connection()
{
    _socket_ptr->close();
    _is_connected = false;
}

void ServerConnection::set_new_server_ip(boost::asio::ip::address server_ip, int port)
{
    if(_is_connected)
    {
        close_connection();
    }

    _server_ip = server_ip;
    _port = port;
    _socket_ptr.reset();
    _socket_ptr = boost::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(_io_service));
}

void ServerConnection::set_new_server_ip(std::string server_ip, int port)
{
    set_new_server_ip(boost::asio::ip::address::from_string(server_ip), port);
}

//template<typename T> void ServerConnection::send_data(T data)


