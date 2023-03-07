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

    _is_read = true;
    boost::asio::streambuf name_size_buffer(NUM_BUFFER_SIZE);
    boost::asio::streambuf data_size_buffer(NUM_BUFFER_SIZE);

    size_t name_size = 0;
    size_t data_size = 0;

    std::string name;
    
    try
    {
        std::cout<<"ksdfmk"<<std::endl;
        auto bytes_transferred = boost::asio::read_until(*_socket_ptr, name_size_buffer, ' ');
        name_size_buffer.commit(bytes_transferred);
        std::istream istream(&name_size_buffer);
        std::cout<<istream.rdbuf()<<"\nkslf"<<std::endl;
        istream>>name_size;
        std::cout<<name_size<<std::endl;
        std::cout<<"kfd,gmdfmgk"<<std::endl;
    }
    catch(const std::exception& e)
    {
        //send smth to server about err
        _is_read = false;
        std::cerr << e.what() << '\n';
        return;
    }

    std::cout<<"sdf,dmsf,"<<std::endl;

    try
    {
        std::cout<<"ksdfmk"<<std::endl;
        auto bytes_transferred = boost::asio::read_until(*_socket_ptr, data_size_buffer, ' ');
        data_size_buffer.commit(bytes_transferred);
        std::istream istream(&data_size_buffer);
        istream>>data_size;      
        std::cout<<data_size<<std::endl;
        std::cout<<"kklk"<<std::endl;
    }
    catch(const std::exception& e)
    {
        //send smth to server about err
        _is_read = false;
        std::cerr << e.what() << '\n';
        std::cout<<"eeee"<<std::endl;
        return;
    }
    std::cout<<",jkjk,"<<std::endl;

    boost::asio::streambuf name_buffer(name_size);

    if(name_size != 0)
    {
        try
        {
            boost::asio::read(*_socket_ptr, name_buffer);  
            std::istream istream(&name_buffer);
            istream>>name;      
            std::cout<<name<<std::endl;
        }
        catch(const std::exception& e)
        {
            //send smth to server about err
            _is_read = false;
            std::cerr << e.what() << '\n';
            return;
        }
    }

    boost::asio::streambuf data_buffer(data_size);
    boost::shared_ptr<std::istream> data_stream_ptr;

    if(data_size != 0)
    {
        try
        {
            boost::asio::read(*_socket_ptr, name_buffer);  
            data_stream_ptr = boost::shared_ptr<std::istream>(new std::istream(&data_buffer));
        }
        catch(const std::exception& e)
        {
            //send smth to server about err
            _is_read = false;
            std::cerr << e.what() << '\n';
            return;
        }
    }

    ReadData _read_data(name, data_stream_ptr);
    read_data_array.push_back(_read_data);
    _is_read = false;
}

void ServerConnection::thread_read_data()
{
    boost::thread(&ServerConnection::read_data, this);
}

