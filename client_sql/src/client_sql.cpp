#include "client_sql.h"


void ClientData::set_new_user(User user)
{
    _user = user;
    verify_user();
}

void ClientData::verify_user()
{
    server_connection.send_data("VERIFY", _user);
}

void ClientData::update_events()
{
    server_connection.send_data("UPDATE EVENTS", "");
}

