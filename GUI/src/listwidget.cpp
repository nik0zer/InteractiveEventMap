#include "../include/listwidget.h"
#include "../include/Dialod.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <ctime>
#include <iostream>
 
ListWidget::ListWidget(QWidget *parent)
    : QWidget(parent) {
 
  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->setSpacing(10);
 
  QHBoxLayout *hbox = new QHBoxLayout(this);
 
  auto& Base = DataBase::get_instance();
  auto events = Base.get_all_events();

  lw = new QListWidget(this);
  
  for(auto& item : Base.get_all_events())
  {lw->addItem(QString::fromStdString(item.get_name()));} 
  
 
  add = new QPushButton("Add event", this);
  rename = new QPushButton("Rename event", this);
  remove = new QPushButton("Remove event", this);
  seeEvent = new QPushButton("See event", this);

  vbox->setSpacing(3);
  vbox->addStretch(1);
  vbox->addWidget(add);
  vbox->addWidget(rename);
  vbox->addWidget(remove);
  vbox->addWidget(seeEvent);
  vbox->addStretch(1);
 
  hbox->addWidget(lw);
  hbox->addSpacing(15);
  hbox->addLayout(vbox);
  
  connect(add, &QPushButton::clicked, this, &ListWidget::addItem);
  connect(rename, &QPushButton::clicked, this, &ListWidget::renameItem);
  connect(remove, &QPushButton::clicked, this, &ListWidget::removeItem);
  connect(seeEvent, &QPushButton::clicked, this, &ListWidget::see);
  
  setLayout(hbox);
}
 
void ListWidget::addItem() { 
    
  QString c_name = QInputDialog::getText(this, "Event", "Enter new event name");
  QString s_name = c_name.simplified();
  
  
  if (!s_name.isEmpty()) {
      
    QString c_info = QInputDialog::getText(this, "Event", "Enter new event info");
    QString s_info = c_info.simplified();
    QString c_address = QInputDialog::getText(this, "Event", "Enter new event address");
    QString s_address = c_address.simplified();
    QString c_date = QInputDialog::getText(this, "Event", "Enter new event date");
    QString s_date = c_date.simplified();
    QString c_time = QInputDialog::getText(this, "Event", "Enter new event time");
    QString s_time = c_time.simplified();

    auto event = Event(1, s_name.toStdString(), s_info.toStdString(), s_address.toStdString(), s_date.toStdString(), s_time.toStdString(), "", std::time(nullptr));
   // std::cout << event << std::endl;
    auto& Base = DataBase::get_instance();
    Base.add_event(event);

    lw->addItem(s_name);
    int r = lw->count() - 1;
    lw->setCurrentRow(r);
  }
}


void ListWidget::renameItem()
{
  QListWidgetItem *curitem = lw->currentItem();
  
  int r = lw->row(curitem);
  QString c_text = curitem->text();
  QString r_text = QInputDialog::getText(this, "Item", 
      "Enter new item", QLineEdit::Normal, c_text);
  
  QString s_text = r_text.simplified();
  
  if (!s_text.isEmpty()) {
      
    QListWidgetItem *item = lw->takeItem(r);
    delete item;
    lw->insertItem(r, s_text);
    lw->setCurrentRow(r);
  }
}


void ListWidget::removeItem() {
    
  int r = lw->currentRow();
 
  if (r != -1) {
      
    QListWidgetItem *item = lw->takeItem(r);
    delete item;
  }
}
 
void ListWidget::clearItems(){
    
  if (lw->count() != 0) {
    lw->clear();
  }
}

void ListWidget::see(){


    int r = lw->currentRow();
    DataBase::get_instance();
    auto events = DataBase::get_instance().get_all_events();

    if (r != -1) { 
      Dialog *dg = new Dialog();
      dg->setEvent(events[r]);
      dg->update();
      dg->show();
  }

}

void ListWidget::updateEventsList()
{
  auto events = DataBase::get_instance().get_all_events();

  for(int it = 0; it < events.size(); it++)
  {
    QString c_text = QString::fromStdString(events[it].get_name());
    QString s_text = c_text.simplified();
    
    if (!s_text.isEmpty()) {
        
      lw->addItem(s_text);
      int r = lw->count() - 1;
      lw->setCurrentRow(r);
    }
  }


    printf("here\n");
    

}

#include <moc_listwidget.cpp>