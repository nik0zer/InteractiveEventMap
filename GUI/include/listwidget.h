#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#pragma once
 
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include "client_sql.h"
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
    QPushButton *AllEvents; 
  
  public:
    ListWidget(QWidget *parent = 0);
    void updateEventsList();
     
  private slots:
    void addItem();
    void renameItem();
    void removeItem();
    void see();
    void synhronize();
};

#endif