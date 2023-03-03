#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

class ServerConnection
{
  private:
    boost::asio::ip::address _server_ip;
    bool _is_connected;
    boost::asio::io_service _io_service;
    int _port;
    boost::shared_ptr<boost::asio::ip::tcp::socket> _socket_ptr;



  public:
    void connect_to_server();
    void set_new_server_ip(std::string server_ip, int port);
    void set_new_server_ip(boost::asio::ip::address server_ip, int port);
    void close_connection();

    template<typename T> void send_data(T data)
    {
      if(!_is_connected)
      {
          try
          {
              connect_to_server();
          }
          catch(...)
          {
              return;
          }
      }
      
      boost::asio::streambuf buffer;
      std::ostream out(&buffer);

      out<<data<<std::endl;
      write(*_socket_ptr ,buffer);
  }

    ServerConnection(std::string server_ip, int port);
    ServerConnection(boost::asio::ip::address server_ip, int port);
};

#endif