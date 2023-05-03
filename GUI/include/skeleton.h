#ifndef SKELETON_H
#define SKELETON_H
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
};

#endif