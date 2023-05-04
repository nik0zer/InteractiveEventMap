#include "network_connection_server.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>


Client::Client(int client_id, std::shared_ptr<boost::thread> client_session_ptr, std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr)
{
    this->client_id = client_id;
    this->client_session_ptr = client_session_ptr;
    _socket_ptr = socket_ptr;
}

Server::Server(int port)
{
    _port  = port;
    _endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port);
    
    for(int i = 0; i < MAX_OF_CLIENTS; i++)
    {
        free_client_id.push_back(i);
    }
}

void Server::client_waiting(void client_session(ClientConnection ClientConnection))
{
    boost::asio::ip::tcp::acceptor acceptor(_io_service, _endpoint);
    while(true)
    {
        std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr(new boost::asio::ip::tcp::socket(_io_service));
        acceptor.accept(*socket_ptr);

        std::shared_ptr<boost::thread> thread_ptr(new boost::thread(client_session, ClientConnection(socket_ptr)));
        clients.push_back(Client(free_client_id[0], thread_ptr, socket_ptr));
        
        free_client_id.erase(free_client_id.begin());

        if(!free_client_id.size())
        {
            client_update();
        }
    }
}

void Server::client_update()
{
    for(std::vector<Client>::iterator i = clients.begin(); i != clients.end();)
    {
        if(!(*i).check_connection())
        {
            free_client_id.push_back((*i).client_id);
            i = clients.erase(i);
        }
        else
        {
            i++;
        }
    }
}

bool Client::check_connection()
{
    return _socket_ptr->is_open();
}

void ClientConnection::send_buffer(std::shared_ptr<boost::asio::streambuf> buffer_ptr)
{
    std::lock_guard<std::mutex> lock(_write_mutex);
    
    size_t written_length = 0;

    size_t buffer_size = (*buffer_ptr).size();

    try
    {
        written_length = write(*_socket_ptr, (*buffer_ptr));
    }
    catch(boost::system::system_error e)
    {
        std::cerr << e.what() << std::endl;
        if(e.code().value() == EPIPE || e.code().value() == ECONNRESET || e.code().value() == END_OF_FILE 
        || e.code().value() == BAD_FILE_DESCRIPTOR)
        {
            if(_socket_ptr->is_open())
            {
                _socket_ptr_mutex.lock();
                _socket_ptr->close();
                _socket_ptr_mutex.unlock();
            }
        }
        
        throw;
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

    

    if(written_length != buffer_size)
    {
        return;
    }
}

std::string ReadData::data_name()
{
    return _data_name;
}

std::shared_ptr<std::vector<char>> ReadData::data_str_ptr()
{
    return _data_str_ptr;
}

boost::thread ClientConnection::thread_read_data(READ_DATA_HANDLER(void read_data_handler(ReadData read_data, ClientConnection* client_connection)))
{
    return boost::thread(&ClientConnection::_thread_read_data, this, READ_DATA_HANDLER(read_data_handler));
}

void ClientConnection::read_data(READ_DATA_HANDLER(void read_data_handler(ReadData read_data, ClientConnection* client_connection)))
{
    std::lock_guard<std::mutex> lock(_read_mutex);

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
        if(e.code().value() == EPIPE || e.code().value() == ECONNRESET || e.code().value() == END_OF_FILE 
        || e.code().value() == BAD_FILE_DESCRIPTOR)
        {
            if(_socket_ptr->is_open())
            {
                _socket_ptr_mutex.lock();
                _socket_ptr->close();
                _socket_ptr_mutex.unlock();
            }
        }
        throw;
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
    READ_DATA_ARRAY(
    read_data_mutex.lock();
    read_data_array.push_back(_read_data);
    read_data_mutex.unlock();)
    READ_DATA_HANDLER(boost::thread(read_data_handler, _read_data, this);)
    
}

void ClientConnection::cycle_read(READ_DATA_HANDLER(void read_data_handler(ReadData read_data, ClientConnection* client_connection)))
{
    while(true)
    {
        try
        {
            read_data(READ_DATA_HANDLER(read_data_handler));
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

boost::thread ClientConnection::thread_cycle_read(READ_DATA_HANDLER(void read_data_handler(ReadData read_data, ClientConnection* client_connection)))
{
    return boost::thread(&ClientConnection::cycle_read, this, READ_DATA_HANDLER(read_data_handler));
}

void ClientConnection::read_data_array_delete_elem(std::vector<ReadData> :: iterator i)
{
    read_data_mutex.lock();
    read_data_array.erase(i);
    read_data_mutex.unlock();
}

bool ClientConnection::is_socket_open()
{
    return _socket_ptr->is_open();
}

void ClientConnection::_thread_read_data(READ_DATA_HANDLER(void read_data_handler(ReadData read_data, ClientConnection* client_connection)))
{
    try
    {
        read_data(READ_DATA_HANDLER(read_data_handler));
    }
    catch(const std::exception& e)
    {
        std::cerr<<e.what()<<std::endl;
    }
}

uint32_t ReadData::data_size()
{
    return this->_data_size;
}

std::string ReadData::data_str()
{
    std::string data;
    for(auto i : *_data_str_ptr)
    {
        data += i;
    }
    return data;
}

