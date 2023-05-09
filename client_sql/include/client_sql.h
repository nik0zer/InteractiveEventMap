#ifndef CLIENT_SQL_H
#define CLIENT_SQL_H

#include "sql_new.h"
#include "network_connection_client.h"
#include <functional>

void handler(ReadData read_data, ServerConnection* server_connection);

class ClientData
{
    private:
    User _user;
    ClientData(User user, std::string ip, int port) : _user(user), data_base(DataBase_Client::get_instance()), 
    server_connection(ip, port) {server_connection.thread_cycle_read(handler);};
    public:
    ServerConnection server_connection;

    void update_events();

    void verify_user();
    void set_new_user(User user);
    DataBase_Client& data_base;

    static ClientData& get_instance(User user, std::string ip, int port)
    {
        static ClientData client_data(user, ip, port);
        return client_data;
    }
};








#endif