#include "network_connection.h"
#include <iterator>

ServerConnection::ServerConnection(std::string server_ip, int port)
{
    _server_ip = boost::asio::ip::address::from_string(server_ip);
    _port = port;
    _is_connected = false;
    _socket_ptr = std::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(_io_service));
}

ServerConnection::ServerConnection(boost::asio::ip::address server_ip, int port)
{
    _server_ip = server_ip;
    _port = port;
    _is_connected = false;
    _socket_ptr = std::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(_io_service));
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
    if(_socket_ptr->is_open())
    {
        _socket_ptr_mutex.lock();
        _socket_ptr->close();
        _socket_ptr_mutex.unlock();
    }
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
    _socket_ptr = std::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(_io_service));
}

void ServerConnection::set_new_server_ip(std::string server_ip, int port)
{
    set_new_server_ip(boost::asio::ip::address::from_string(server_ip), port);
}

void ServerConnection::read_data()
{
    std::lock_guard<std::mutex> lock(_read_mutex);

    if(!_is_connected)
    {
        connect_to_server();
    }

    boost::asio::streambuf data_buffer;
    std::shared_ptr<std::vector<char>> data_ptr(new std::vector<char>());
    uint32_t data_size;
    std::string name;
    
    try
    {
        boost::asio::read(*_socket_ptr, data_buffer, boost::asio::transfer_exactly(sizeof(uint32_t)));
        bytes_from_uint32_t name_size; 
        for(int i = 0; i < sizeof(uint32_t) && data_buffer.sgetc() != EOF; i++)
        {
            name_size.bytes[i] = data_buffer.sgetc();
            data_buffer.snextc();
        }

        boost::asio::read(*_socket_ptr, data_buffer, boost::asio::transfer_exactly(name_size.uint));

        name = std::string((std::istreambuf_iterator<char>(&data_buffer)), std::istreambuf_iterator<char>());

        boost::asio::read(*_socket_ptr, data_buffer, boost::asio::transfer_exactly(sizeof(uint32_t)));
        bytes_from_uint32_t data_size; 

        for(int i = 0; i < sizeof(uint32_t) && data_buffer.sgetc() != EOF; i++)
        {
            data_size.bytes[i] = data_buffer.sgetc();
            data_buffer.snextc();
        }

        boost::asio::read(*_socket_ptr, data_buffer, boost::asio::transfer_exactly(data_size.uint));

        for(int i = 0; i < data_size.uint && data_buffer.sgetc() != EOF; i++)
        {
            data_ptr->push_back(data_buffer.sgetc());
            data_buffer.snextc();
        }


    }
    catch(boost::system::system_error e)
    {
        std::cerr << e.what() << std::endl;
        if(e.code().value() == EPIPE || e.code().value() == ECONNRESET || e.code().value() == END_OF_FILE)
        {
            if(_socket_ptr->is_open())
            {
                _socket_ptr_mutex.lock();
                _socket_ptr->close();
                _socket_ptr_mutex.unlock();
            }
            
            throw(e);
            return;
        }
        
        return;
    }
    catch(const std::exception& e)
    {
        std::cerr<<e.what()<<std::endl;
        throw e;
        return;
    }
    catch(...)
    {
        throw;
        return;
    }

    ReadData _read_data(name, data_size, data_ptr);
    read_data_mutex.lock();
    read_data_array.push_back(_read_data);
    read_data_mutex.unlock();
    
}

boost::thread ServerConnection::thread_read_data()
{
    return boost::thread(&ServerConnection::_thread_read_data, this);
}

//dont use destructor if thread funcs is used in the moment
ServerConnection::~ServerConnection()
{
    if(_is_connected)
    {
        close_connection();
    }
}

std::string ReadData::data_name()
{
    return _data_name;
}

std::shared_ptr<std::vector<char>> ReadData::data_str_ptr()
{
    return (_data_str_ptr);
}

uint32_t ReadData::data_size()
{
    return this->_data_size;
}

void ServerConnection::send_buffer(std::shared_ptr<boost::asio::streambuf> buffer_ptr)
{
    std::lock_guard<std::mutex> lock(_write_mutex);
    
    if(!_is_connected)
    {
        try
        {
            connect_to_server();
        }
        catch(const std::exception& e)
        {
            std::cerr<<e.what()<<std::endl;
            throw e;
            return;
        }
        catch(...)
        {
            throw;
            return;
        }
    }
    
    
    size_t written_length = 0;

    size_t buffer_size = (*buffer_ptr).size();

    try
    {
        written_length = write(*_socket_ptr, (*buffer_ptr));
    }
    catch(boost::system::system_error e)
    {
        std::cerr << e.what() << std::endl;
        if(e.code().value() == EPIPE || e.code().value() == ECONNRESET || e.code().value() == END_OF_FILE)
        {
            if(_socket_ptr->is_open())
            {
                _socket_ptr_mutex.lock();
                _socket_ptr->close();
                _socket_ptr_mutex.unlock();
            }
            
            throw(e);
            return;
        }
        return;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        throw e;
        return;
    }
    catch(...)
    {
        throw;
        return;
    }

    
}

void ServerConnection::read_data_array_delete_elem(std::vector<ReadData> :: iterator i)
{
    read_data_mutex.lock();
    read_data_array.erase(i);
    read_data_mutex.unlock();
}

void ServerConnection::_thread_read_data()
{
    try
    {
        read_data();
    }
    catch(const std::exception& e)
    {
        std::cerr<<e.what()<<std::endl;
    }
}

void ServerConnection::cycle_read()
{
    while(true)
    {
        try
        {
            read_data();
        }
        catch(const std::exception& e)
        {
            std::cerr<<e.what()<<std::endl;
            return;
        }
        catch(...)
        {
            return;
        }
    }
}

boost::thread ServerConnection::thread_cycle_read()
{
    return boost::thread(&ServerConnection::cycle_read, this);
}

