#pragma once
 
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include "sql_tools.h"
#include <vector>
 
class ListWidget : public QWidget {
    
  Q_OBJECT

  private:
    QListWidget *lw;
    QPushButton *add;
    QPushButton *rename;
    QPushButton *remove;
    QPushButton *removeAll; 
    QPushButton *seeEvent; 

    std::vector<Event> events_;

    

 
  public:
    ListWidget(QWidget *parent = 0);
    void setEventVector(std::vector<Event> events);
    void updateEventsList();
    
 
  private slots:
    void addItem();
    void renameItem();
    void removeItem();
    void clearItems();
    void see();
 
  



};