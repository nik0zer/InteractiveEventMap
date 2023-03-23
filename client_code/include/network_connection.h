#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

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
    template<typename T> void data_to_buffer(T data, std::shared_ptr<boost::asio::streambuf> buffer_ptr)
    {
      std::ostream out(buffer_ptr.get());
      out<<data<<std::endl;
    }
    void send_buffer(std::shared_ptr<boost::asio::streambuf> buffer_ptr);

  public:
    std::vector<ReadData> read_data_array;
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
      std::shared_ptr<boost::asio::streambuf> buffer_ptr = std::shared_ptr<boost::asio::streambuf>(new boost::asio::streambuf);
      data_to_buffer(data, buffer_ptr);
      send_buffer(buffer_ptr);
    }

    ServerConnection(std::string server_ip, int port);
    ServerConnection(boost::asio::ip::address server_ip, int port);
    ~ServerConnection();
};

#endif