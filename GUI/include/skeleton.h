#pragma once
 
#include <QMainWindow>
#include <QApplication>
#include <vector>
#include "sql_tools.h"
#include "listwidget.h"

 
class Skeleton : public QMainWindow {
    
  Q_OBJECT 

  private:
  std::vector<Event> events_; 
  ListWidget *list;
 
  public:
    Skeleton(QWidget *parent = 0);
    void setEventVector(std::vector<Event> events){events_ = events;};
    std::vector<Event> getEventVector() {return events_;};
    void updateEventsList(); 

  
    
};