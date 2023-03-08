#include "network_connection.h"

ServerConnection::ServerConnection(std::string server_ip, int port)
{
    _server_ip = boost::asio::ip::address::from_string(server_ip);
    _port = port;
    _is_connected = false;
    _is_read = false;
    _is_written = false;
    _socket_ptr = boost::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(_io_service));
}

ServerConnection::ServerConnection(boost::asio::ip::address server_ip, int port)
{
    _server_ip = server_ip;
    _port = port;
    _is_connected = false;
    _is_read = false;
    _is_written = false;
    _socket_ptr = boost::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(_io_service));
}

void ServerConnection::connect_to_server()
{
    boost::asio::ip::tcp::endpoint endpoint(_server_ip, _port);
    try
    {
        _socket_ptr->connect(endpoint);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
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

void ServerConnection::read_data()
{
    while(_is_read)
    {
        std::cout<<".."<<std::endl;
    }

    if(!_is_connected)
    {
        connect_to_server();
    }

    _is_read = true;
    boost::asio::streambuf data_buffer;
    boost::shared_ptr<std::istream> data_stream_ptr;
    int name;
    
    try
    {
        boost::asio::read_until(*_socket_ptr, data_buffer, "\n");  
        std::cout<<"qqq"<<std::endl;
        std::cout<<"qqq"<<std::endl;
        //data_stream_ptr = boost::shared_ptr<std::istream>(new std::istream(&data_buffer));
        std::istream data_stream(&data_buffer);
        //std::cout<<data_stream_ptr->rdbuf()<<std::endl;
        data_stream>>name;
        std::cout<<data_stream.rdbuf()<<"    ldsflfk"<<std::endl;
        std::cout<<"1234thy"<<std::endl;
        
        std::cout<<name<<"lskf"<<std::endl;
    }
    catch(const std::exception& e)
    {
        //send smth to server about err
        _is_read = false;
        std::cerr << e.what() << '\n';
        return;
    }

    ReadData _read_data(std::to_string(name), data_stream_ptr);
    read_data_array.push_back(_read_data);
    _is_read = false;
}

void ServerConnection::thread_read_data()
{
    boost::thread(&ServerConnection::read_data, this);
}

