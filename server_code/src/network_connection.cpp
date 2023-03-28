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
    std::cout<<port<<std::endl;
    _endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port);
    
    for(int i = 0; i < MAX_OF_CLIENTS; i++)
    {
        free_client_id.push_back(i);
    }
}

void Server::client_waiting(void client_session(Client_connection client_connection))
{
    boost::asio::ip::tcp::acceptor acceptor(_io_service, _endpoint);
    while(true)
    {
        std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr(new boost::asio::ip::tcp::socket(_io_service));
        acceptor.accept(*socket_ptr);

        std::cout<<"1"<<std::endl;
        std::shared_ptr<boost::thread> thread_ptr(new boost::thread(client_session, Client_connection(socket_ptr)));

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

void Client_connection::send_buffer(std::shared_ptr<boost::asio::streambuf> buffer_ptr)
{
    while(_is_written)
    {
    }
    _is_written = true;  
    
    size_t written_length = 0;

    size_t buffer_size = (*buffer_ptr).size();

    try
    {
        written_length = write(*_socket_ptr, (*buffer_ptr));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        _is_written = false;
        return;
    }

    _is_written = false;

    if(written_length != buffer_size)
    {
        return;
    }
}

