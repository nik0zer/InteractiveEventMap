#include "network_connection.h"
#include <QApplication>
#include "Mainwindow.h"
#include "reg_window.h"
#include "auth_window.h"
#include "sql_tools.h"
#include <vector>
#define IP "127.0.0.1"
#define PORT 2001

int main(int argc, char *argv[])
{
    ServerConnection server_connection(IP, PORT);
    server_connection.send_data("send_me_actual_persons", DataBase::get_instance().get_last_edit_time_persons);
    server_connection.send_data("send_me_actual_events", DataBase::get_instance().get_last_edit_time_events);

    
    QApplication app(argc, argv);  
 
    MainWindow window(nullptr);
    
    window.display();
  
    return app.exec();
}