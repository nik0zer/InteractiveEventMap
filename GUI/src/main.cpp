#include <QApplication>
#include "../include/Mainwindow.h"
#include "../include/reg_window.h"
#include "../include/auth_window.h"
#include "sql_tools.h"
#include <vector>


 
int main(int argc, char *argv[]) {
    
  QApplication app(argc, argv);  
    
 // Person per("first", "12345"); 
  //DataBase::get_instance().add_person(per);

  // Event event(10, "DF", "openAirFestival", "MIPT", "30.04.2023", "18:00", "MIPT DIR", std::time(nullptr));
  // DataBase::get_instance().add_event(event);
 
  MainWindow window(nullptr);
  /*window.setPersonVec(pers);
  window.setEventVec(events);*/
 
  window.setWindowTitle("Auth_window");
  window.display();
  
  return app.exec();

}