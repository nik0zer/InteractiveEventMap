#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include <mutex>
#include <vector>

enum ERRORS
{
    END_OF_FILE = 2
};

union bytes_from_uint32_t
{
  uint32_t uint;
  char bytes[sizeof(uint32_t)];
};

class ReadData
{
  private:
    std::string _data_name;
    uint32_t _data_size;
    std::shared_ptr<std::vector<char>> _data_str_ptr;

  public:
    std::string data_name();
    std::shared_ptr<std::vector<char>> data_str_ptr();
    uint32_t data_size();
    ReadData(std::string data_name, uint32_t data_size, std::shared_ptr<std::vector<char>> data_str_ptr) :
    _data_name(data_name), _data_size(data_size), _data_str_ptr(data_str_ptr){};
};


class ServerConnection
{
  private:
    boost::asio::ip::address _server_ip;
    bool _is_connected;
    boost::asio::io_service _io_service;
    int _port;
    std::shared_ptr<boost::asio::ip::tcp::socket> _socket_ptr;
    std::mutex _write_mutex;
    std::mutex _read_mutex;
    std::mutex _socket_ptr_mutex;
    

    template<typename T> void data_to_buffer(std::string name, T data, std::shared_ptr<boost::asio::streambuf> buffer_ptr)
    {
        std::ostream out(buffer_ptr.get());
        uint32_t name_size = name.size();
        bytes_from_uint32_t name_size_byte;
        name_size_byte.uint = name_size;
        
        std::string data_str = data;
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

    void send_buffer(std::shared_ptr<boost::asio::streambuf> buffer_ptr);
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

  public:
    std::mutex read_data_mutex;
    std::vector<ReadData> read_data_array;
    void connect_to_server();
    void set_new_server_ip(std::string server_ip, int port);
    void set_new_server_ip(boost::asio::ip::address server_ip, int port);
    void close_connection();
    void read_data();
    void read_data_array_delete_elem(std::vector<ReadData> :: iterator i);
    boost::thread thread_read_data();

    template<typename T> boost::thread thread_send_data(std::string name, T data)
    {
      return boost::thread(&ServerConnection::_thread_send_data<T>, this, name, data);
    }

    template<typename T> void send_data(std::string name, T data)
    {
      std::shared_ptr<boost::asio::streambuf> buffer_ptr(new boost::asio::streambuf);
      data_to_buffer(name, data, buffer_ptr);
      send_buffer(buffer_ptr);
    }

    ServerConnection(std::string server_ip, int port);
    ServerConnection(boost::asio::ip::address server_ip, int port);
    ServerConnection(const ServerConnection& server) = delete;
    ServerConnection operator=(const ServerConnection& server) = delete;
    ~ServerConnection();
};


#endif