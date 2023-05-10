#ifndef CLIENT_SQL_H
#define CLIENT_SQL_H

#include "sql_new.h"
#include "network_connection_client.h"
#include <functional>

enum
{
    NOT_CHECKED = -1,
    VERIFIED = 1,
    NOT_VERIFIED = 0
};

void handler(ReadData read_data, ServerConnection* server_connection);

class ClientData
{
    private:
    User _user;

    time_t synch_time;
    

    ClientData() : data_base(DataBase_Client::get_instance()) {synch_time = 0;};
    public:
    std::mutex verify_mutex;
    std::shared_ptr<ServerConnection> server_connection_ptr;
    int verification;
    
    void add_server_connection_ptr(std::shared_ptr<ServerConnection> server_connection_ptr);
    void update_events();

    int verify_user();
    void registration(User user);
    void set_new_user(User user);
    DataBase_Client& data_base;

    static ClientData& get_instance()
    {
        static ClientData client_data;
        return client_data;
    }
};








#endif