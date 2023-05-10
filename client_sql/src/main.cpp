#include "client_sql.h"
#define IP "127.0.0.1"
#define PORT 2001

#include <QApplication>
#include "../include/Mainwindow.h"
#include "../include/reg_window.h"
#include "../include/auth_window.h"
#include "client_sql.h"
#include <vector>

int main(int argc, char *argv[])
{
    std::shared_ptr<ServerConnection> server_connection_ptr(new ServerConnection(IP, PORT));
    ClientData& client_data = ClientData::get_instance();
    client_data.add_server_connection_ptr(server_connection_ptr);
    client_data.data_base.add_event(Event("1", "1", "1"));
    client_data.update_events();
    ClientData::get_instance().data_base.print_all_events();
   
    QApplication app(argc, argv);  
 
    MainWindow window(nullptr);
    
    window.display();
    
    return app.exec();
}