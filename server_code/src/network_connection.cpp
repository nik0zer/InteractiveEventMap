#include "network_connection.h"
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
        if(e.code().value() == EPIPE || e.code().value() == ECONNRESET || e.code().value() == END_OF_FILE)
        {
            if(_socket_ptr->is_open())
            {
                _socket_ptr_mutex.lock();
                _socket_ptr->close();
                _socket_ptr_mutex.unlock();
            }
        }
        
        throw(e);
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

std::string ReadData::data_str()
{
    return (*_data_str_ptr);
}

boost::thread ClientConnection::thread_read_data()
{
    return boost::thread(&ClientConnection::_thread_read_data, this);
}

void ClientConnection::read_data()
{
    std::lock_guard<std::mutex> lock(_read_mutex);

    if(!(_socket_ptr->is_open()))
    {
        throw std::exception();
        return;
    }
    
    boost::asio::streambuf data_buffer;
    std::istream data_stream(&data_buffer);
    std::shared_ptr<std::string> data_str_ptr(new std::string());
    std::string name;
    
    try
    {
        boost::asio::read_until(*_socket_ptr, data_buffer, "\n");
        data_stream >> name;
        data_stream.ignore(1);
        std::getline(data_stream, (*data_str_ptr), '\n');
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
        }
        throw e;
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

    ReadData _read_data(name, data_str_ptr);
    read_data_mutex.lock();
    read_data_array.push_back(_read_data);
    read_data_mutex.unlock(); 
}

void ClientConnection::cycle_read()
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

boost::thread ClientConnection::thread_cycle_read()
{
    return boost::thread(&ClientConnection::cycle_read, this);
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

void ClientConnection::_thread_read_data()
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

