#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

enum SERVERCONNECTION_CONSTANTS
{
    NUM_BUFFER_SIZE = 512
};

class ReadData
{
  public:
    std::string _data_name;
    std::shared_ptr<std::string> _data_str_ptr;

  public:
    ReadData(std::string data_name, std::shared_ptr<std::string> data_str_ptr) :
    _data_name(data_name), _data_str_ptr(data_str_ptr){};
};

class ServerConnection
{
  private:
    boost::asio::ip::address _server_ip;
    bool _is_connected;
    boost::asio::io_service _io_service;
    int _port;
    std::shared_ptr<boost::asio::ip::tcp::socket> _socket_ptr;
    bool _is_written;
    bool _is_read;

  public:
    std::vector<ReadData> read_data_array;
    

  public:
    void connect_to_server();
    void set_new_server_ip(std::string server_ip, int port);
    void set_new_server_ip(boost::asio::ip::address server_ip, int port);
    void close_connection();
    void read_data();
    boost::thread thread_read_data();

    template<typename T> boost::thread thread_send_data(T data)
    {
      return boost::thread(&ServerConnection::send_data<T>, this, data);
    }
    
    template<typename T> void send_data(T data)
    {
      while(_is_written)
      {
      }

      _is_written = true;
      if(!_is_connected)
      {
          try
          {
            connect_to_server();
          }
          catch(const std::exception& e)
          {
            std::cerr << e.what() << '\n';
            _is_written = false;
            return;
          }
      }
      
      boost::asio::streambuf buffer;
      std::ostream out(&buffer);
      size_t written_length = 0;
      out<<data<<std::endl;

      size_t buffer_size = buffer.size();

      try
      {
        written_length = write(*_socket_ptr, buffer);
      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << '\n';
        _is_written = false;
        return;
      }

      _is_written = false;

      if(written_length != buffer_size)
      {
        return;
      }
    }

    ServerConnection(std::string server_ip, int port);
    ServerConnection(boost::asio::ip::address server_ip, int port);
};

#endif