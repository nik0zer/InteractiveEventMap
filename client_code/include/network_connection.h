#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include <mutex>

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


class ServerConnection
{
  private:
    boost::asio::ip::address _server_ip;
    bool _is_connected;
    boost::asio::io_service _io_service;
    int _port;
    std::shared_ptr<boost::asio::ip::tcp::socket> _socket_ptr;
    std::mutex write_mutex;
    std::mutex read_mutex;
    std::mutex read_data_mutex;

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
    void read_data_array_delete_elem(std::vector<ReadData> :: iterator i);
    std::thread thread_read_data();

    template<typename T> std::thread thread_send_data(T data)
    {
      return std::thread(&ServerConnection::send_data<T>, this, data);
    }

    template<typename T> void send_data(T data)
    {
      std::shared_ptr<boost::asio::streambuf> buffer_ptr(new boost::asio::streambuf);
      data_to_buffer(data, buffer_ptr);
      send_buffer(buffer_ptr);
    }

    ServerConnection(std::string server_ip, int port);
    ServerConnection(boost::asio::ip::address server_ip, int port);
    ServerConnection(const ServerConnection& server) = delete;
    ServerConnection operator=(const ServerConnection& server) = delete;
    ~ServerConnection();
};


#endif