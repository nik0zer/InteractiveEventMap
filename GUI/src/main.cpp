#include <QApplication>
#include "../include/Mainwindow.h"
#include "../include/reg_window.h"
#include "../include/auth_window.h"
#include "sql_tools.h"
#include <vector>

int main(int argc, char *argv[]) 
{
  QApplication app(argc, argv);  
 
  MainWindow window(nullptr);
 
  window.setWindowTitle("Auth_window");
  window.display();
  
  return app.exec();
}