#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include <mutex>

/**
 * @brief Enum with info about max clients 
 * 
 */
enum
{
    MAX_OF_CLIENTS = 4
};

/**
 * @brief Enum with info about error codes
 * 
 */
enum ERRORS
{
    END_OF_FILE = 2
};

/**
 * @brief Union for conversions from uint32_t to byte array and back
 * 
 */
union bytes_from_uint32_t
{
  uint32_t uint;
  char bytes[sizeof(uint32_t)];
};

/**
 * @brief Сlass containing data read from a message
 * 
 */
class ReadData
{
  private:
    std::string _data_name;
    uint32_t _data_size;
    std::shared_ptr<std::vector<char>> _data_str_ptr;

  public:
    /**
     * @brief return name of datagramm
     * 
     * @return std::string 
     */
    std::string data_name();

    /**
     * @brief return char vector contains message data
     * 
     * @return std::shared_ptr<std::vector<char>> 
     */
    std::shared_ptr<std::vector<char>> data_str_ptr();

    /**
     * @brief return size of message data
     * 
     * @return uint32_t 
     */
    uint32_t data_size();

    /**
     * @brief Construct a new Read Data object
     * 
     * @param data_name name of message
     * @param data_size size of message data
     * @param data_str_ptr char vector contains message data
     */
    ReadData(std::string data_name, uint32_t data_size, std::shared_ptr<std::vector<char>> data_str_ptr) :
    _data_name(data_name), _data_size(data_size), _data_str_ptr(data_str_ptr){};
};

/**
 * @brief Class with information about connected client
 * 
 */
class Client
{
    private:
        std::shared_ptr<boost::asio::ip::tcp::socket> _socket_ptr;
    public:
        int client_id;
        std::shared_ptr<boost::thread> client_session_ptr;

        /**
         * @brief Construct a new Client object
         * 
         * @param client_id client id
         * @param client_session_ptr pointer to client session thread
         * @param socket_ptr pointer to socket connected with client
         */
        Client(int client_id, std::shared_ptr<boost::thread> client_session_ptr, std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr);
        bool check_connection();
};

/**
 * @brief Сlass that provides a connection to the client
 * 
 */
class ClientConnection
{
    private:
        std::shared_ptr<boost::asio::ip::tcp::socket> _socket_ptr;
        std::mutex _write_mutex;
        std::mutex _read_mutex;
        std::mutex _socket_ptr_mutex;


        void _thread_read_data();
        template<typename T> void _thread_send_data(std::string name, T data)
        {
            try
            {
                send_data<T>(name, data);
            }
            catch(const std::exception& e)
            {
                std::cerr<<e.what()<<std::endl;
            }
        }

        /**
         * @brief send buffer with data to client
         * 
         * @param buffer_ptr pointer to buffer with data
         */
        void send_buffer(std::shared_ptr<boost::asio::streambuf> buffer_ptr);

        /**
         * @brief data to the buffer using the ostream stream
         * 
         * @tparam T class contains data to transfer
         * @param name name of message
         * @param data message data (must have cast to std::string)
         * @param buffer_ptr pointer to datagramm buffer
         */
        template<typename T> void data_to_buffer(std::string name, T data, std::shared_ptr<boost::asio::streambuf> buffer_ptr)
        {
            std::ostream out(buffer_ptr.get());
            uint32_t name_size = name.size();
            bytes_from_uint32_t name_size_byte;
            name_size_byte.uint = name_size;
            
            std::string data_str = (std::string)data;
            uint32_t data_size = data_str.size();
            bytes_from_uint32_t data_size_byte;
            data_size_byte.uint = data_size;
            for(int i = 0; i < sizeof(uint32_t); i++)
            {
                out<<name_size_byte.bytes[i];
            }
            out<<name;

            for(int i = 0; i < sizeof(uint32_t); i++)
            {
                out<<data_size_byte.bytes[i];
            }
            out<<data_str;
        }
    
    public:
        /**
         * @brief mutex to access read data array
         * 
         */
        std::mutex read_data_mutex;
        
        /**
         * @brief Construct a new Client Connection object
         * 
         * @param socket_ptr pointer to socket connected with client
         */
        ClientConnection(std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr) : 
        _socket_ptr(socket_ptr) {};

        /**
         * @brief Construct a new Client Connection object
         * 
         * @param ClientConnection instance of ClientConnection class
         */
        ClientConnection(const ClientConnection& ClientConnection) : _socket_ptr(ClientConnection._socket_ptr) {};

        /**
         * @brief checks the socket for open
         * 
         * @return true if socket open
         * @return false if socket close
         */
        bool is_socket_open();

        /**
         * @brief array with read data from client
         * 
         */
        std::vector<ReadData> read_data_array;

        /**
         * @brief read data from server to read array
         * 
         */
        void read_data();

        /**
         * @brief cyclically reads server messages
         * 
         */
        void cycle_read();

        /**
         * @brief cyclically reads server messages in separated thread
         * 
         * @return boost::thread cyclically reading thread
         */
        boost::thread thread_cycle_read();

        /**
         * @brief read data in a separated tread
         * 
         * @return boost::thread reading thread
         */
        boost::thread thread_read_data();

        /**
         * @brief delete elem from read data array by iterator
         * 
         * @param i iterator of the deleted element
         */
        void read_data_array_delete_elem(std::vector<ReadData> :: iterator i);
        
        /**
         * @brief send message to server in a separated thread
         * 
         * @tparam T type of data for message
         * @param name message name
         * @param data message data
         * @return boost::thread send message thread
         */
        template<typename T> boost::thread thread_send_data(std::string name, T data)
        {
            return boost::thread(&ClientConnection::_thread_send_data<T>, this, name, data);
        }

        /**
         * @brief send message to server
         * 
         * @tparam T type of data for message
         * @param name message name
         * @param data message data
         */
        template<typename T> void send_data(std::string name, T data)
        {
            std::shared_ptr<boost::asio::streambuf> buffer_ptr(new boost::asio::streambuf);
            data_to_buffer(name, data, buffer_ptr);
            send_buffer(buffer_ptr);
        }
};


//client_session function must close socket after all operations
//if an exception has occurred (for example, disconnection from the client side, the socket must also be closed)

/**
 * @brief Class which control clients connection to server
 * 
 */
class Server
{
    private:
        int _port; 
        boost::asio::io_service _io_service;
        boost::asio::ip::tcp::endpoint _endpoint;

    public:
        /**
         * @brief array of clients connected to server
         * 
         */
        std::vector<Client> clients;

        /**
         * @brief array of free clients id
         * 
         */
        std::vector<int> free_client_id;

        /**
         * @brief Construct a new Server object
         * 
         * @param port port for waiting for clients
         */
        Server(int port);
        Server(const Server& server) = delete;
        Server& operator=(const Server& server) = delete;

        /**
         * @brief function is infinitely waiting for the connection of the sockets
         * 
         * @param client_session function that will be called to interact with the client after its connection
         */
        void client_waiting(void client_session(ClientConnection ClientConnection));

        /**
         * @brief function removing inactive clients
         * function removing inactive clients from the list of clients and returning their ids to the list of free ones
         */
        void client_update();
};



#endif