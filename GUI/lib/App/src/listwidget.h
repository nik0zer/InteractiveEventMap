#pragma once
 
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
 
class ListWidget : public QWidget {
    
  Q_OBJECT
 
  public:
    ListWidget(QWidget *parent = 0);
 
  private slots:
    void addItem();
    void renameItem();
    void removeItem();
    void clearItems();
    void see();
 
  private:
    QListWidget *lw;
    QPushButton *add;
    QPushButton *rename;
    QPushButton *remove;
    QPushButton *removeAll; 
    QPushButton *seeEvent; 
};