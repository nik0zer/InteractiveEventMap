#ifndef SERVER_SQL_H
#define SERVER_SQL_H
#include "sql_new.h"
#include "network_connection_server.h"

void client_session(ClientConnection client_connection);

void handler(ReadData read_data, ClientConnection* client_connection);


#endif