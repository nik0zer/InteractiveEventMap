#include "../include/skeleton.h"
#include "../include/listwidget.h"

#include <QToolBar>
#include <QIcon>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
 
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
        
  connect(quit2, &QAction::triggered, qApp, &QApplication::quit);

  list = new ListWidget(this);
  
  
  setCentralWidget(list); // помещаем созданный виджет в центр виджета QMainWindow
 
  statusBar()->showMessage("Ready"); // показываем в нижней панели приложения сообщение "Ready"
}

#include <moc_skeleton.cpp>