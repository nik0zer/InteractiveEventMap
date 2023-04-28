#include <QApplication>
#include "../lib/Reg_form/src/Mainwindow.h"
#include "../lib/Reg_form/src/reg_window.h"
#include "../lib/Reg_form/src/auth_window.h"


 
int main(int argc, char *argv[]) {
    
  QApplication app(argc, argv);  
    
  MainWindow window;
 
  window.setWindowTitle("Auth_window");
  window.display();
  
  return app.exec();
}