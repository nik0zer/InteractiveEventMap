#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include <mutex>

enum
{
    MAX_OF_CLIENTS = 4
};

enum ERRORS
{
    END_OF_FILE = 2
};

class ReadData
{
  private:
    std::string _data_name;
    std::shared_ptr<std::string> _data_str_ptr;

  public:
    std::string data_name();
    std::string data_str();
    ReadData(std::string data_name, std::shared_ptr<std::string> data_str_ptr) :
    _data_name(data_name), _data_str_ptr(data_str_ptr){};
};

class Client
{
    private:
        std::shared_ptr<boost::asio::ip::tcp::socket> _socket_ptr;
    public:
        int client_id;
        std::shared_ptr<boost::thread> client_session_ptr;
        Client(int client_id, std::shared_ptr<boost::thread> client_session_ptr, std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr);
        bool check_connection();
};

class ClientConnection
{
    private:
        std::shared_ptr<boost::asio::ip::tcp::socket> _socket_ptr;
        std::mutex _write_mutex;
        std::mutex _read_mutex;
        std::mutex read_data_mutex;
        void _thread_read_data();
        template<typename T> void data_to_buffer(T data, std::shared_ptr<boost::asio::streambuf> buffer_ptr)
        {
            std::ostream out(buffer_ptr.get());
            out<<data<<std::endl;
        }
        void send_buffer(std::shared_ptr<boost::asio::streambuf> buffer_ptr);

        template<typename T> void _thread_send_data(T data)
        {
            try
            {
                send_data<T>(data);
            }
            catch(const std::exception& e)
            {
                std::cout<<e.what()<<std::endl;
            }
        }
        
    
    public:
        ClientConnection(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr) : 
        _socket_ptr(socket_ptr) {};
        ClientConnection(const ClientConnection& ClientConnection) : _socket_ptr(ClientConnection._socket_ptr) {};
        bool is_socket_open();


        std::vector<ReadData> read_data_array;
        void read_data();
        void cycle_read();
        boost::thread thread_cycle_read();
        boost::thread thread_read_data();
        void read_data_array_delete_elem(std::vector<ReadData> :: iterator i);
        
        template<typename T> boost::thread thread_send_data(T data)
        {
            return boost::thread(&ClientConnection::send_data<T>, this, data);
        }

        template<typename T> void send_data(T data)
        {
            std::shared_ptr<boost::asio::streambuf> buffer_ptr(new boost::asio::streambuf);
            data_to_buffer(data, buffer_ptr);
            send_buffer(buffer_ptr);
        }
};


//client_session function must close socket after all operations
//if an exception has occurred (for example, disconnection from the client side, the socket must also be closed)

class Server
{
    private:
        int _port; 
        boost::asio::io_service _io_service;
        boost::asio::ip::tcp::endpoint _endpoint;

    public:
        std::vector<Client> clients;
        std::vector<int> free_client_id;
        Server(int port);
        Server(const Server& server) = delete;
        Server& operator=(const Server& server) = delete;
        void client_waiting(void client_session(ClientConnection ClientConnection));
        void client_update();
};



#endif