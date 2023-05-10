#ifndef SKELETON_H
#define SKELETON_H
#pragma once
 
#include <QMainWindow>
#include <QApplication>
#include <vector>
#include "client_sql.h"
#include "listwidget.h"

 
class Skeleton : public QMainWindow {
    
  Q_OBJECT 

  private:
  std::vector<Event> events_; 
  ListWidget *list;
 
  public:
    Skeleton(QWidget *parent = 0);

  private slots:
    void  AboutDataShow();
};

#endif