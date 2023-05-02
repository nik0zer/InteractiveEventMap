#include <QApplication>
#include "../include/Mainwindow.h"
#include "../include/reg_window.h"
#include "../include/auth_window.h"
#include "../include/pearsons.h"
#include <vector>


 
int main(int argc, char *argv[]) {
    
  QApplication app(argc, argv);  
    
  Person per(1, "first", "12345"); 
  std::vector<Person> pers; //  нужна функция  std::vector<Person> getAllPersons()
  pers.push_back(per);

  Event event("DF", "openAirFestival", "MIPT", "30.04.2023", "18:00", "MIPT DIR", NULL);
  std::vector<Event> events; //нужна функция std::vector<Event> getAllEvents();
  events.push_back(event);

  MainWindow window(nullptr);
  window.setPersonVec(pers);
  window.setEventVec(events);
 
  window.setWindowTitle("Auth_window");
  window.display();
  
  return app.exec();
}