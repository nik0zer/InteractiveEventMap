#include "../include/skeleton.h"
#include "../include/listwidget.h"
#include "Dialod.h"

#include <QToolBar>
#include <QIcon>
#include <QPixmap>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QDialog>
#include <QPushButton>
 
Skeleton::Skeleton(QWidget *parent)
    : QMainWindow(parent) {
  
  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, Qt::black);
  this->setAutoFillBackground(true);
  this->setPalette(pal);
        
  QPixmap quitpix("../Icons/Quit.png");
 
  QAction *quit = new QAction("&Quit", this);

  QToolBar *toolbar = addToolBar("main toolbar");
  QAction *quit2 = toolbar->addAction(QIcon(quitpix), "Quit Application");
  QAction *about = toolbar->addAction("About");
  
 
  connect(quit2, &QAction::triggered, qApp, &QApplication::quit);
  connect(about, &QAction::triggered, this, &Skeleton::AboutDataShow);
  

  list = new ListWidget(this);
  
  setCentralWidget(list); 
  statusBar()->showMessage("Ready"); 
}

void Skeleton::AboutDataShow()
{
  printf("here3\n");
  Dialog *dg = new Dialog();
  dg->setAbout();
  dg->show();
}
#include <moc_skeleton.cpp>