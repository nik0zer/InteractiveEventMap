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
    QApplication app(argc, argv);  
 
    MainWindow window(nullptr);
    
    window.display();
    
    return app.exec();
}