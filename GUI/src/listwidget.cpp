#include "listwidget.h"
#include "Dialod.h"
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
 
  
  auto events = ClientData::get_instance().data_base.get_all_events();

  lw = new QListWidget(this);
  
  for(auto& item : events)
  {lw->addItem(QString::fromStdString(item.get_name()));} 
  
 
  add = new QPushButton("Add event", this);
  rename = new QPushButton("Rename event", this);
  remove = new QPushButton("Remove event", this);
  seeEvent = new QPushButton("See event", this);
  AllEvents = new QPushButton("Synchronize", this);

  vbox->setSpacing(3);
  vbox->addStretch(1);
  vbox->addWidget(add);
  vbox->addWidget(rename);
  vbox->addWidget(remove);
  vbox->addWidget(seeEvent);
  vbox->addWidget(AllEvents);
  vbox->addStretch(1);
 
  hbox->addWidget(lw);
  hbox->addSpacing(15);
  hbox->addLayout(vbox);
  
  connect(add, &QPushButton::clicked, this, &ListWidget::addItem);
  connect(rename, &QPushButton::clicked, this, &ListWidget::renameItem);
  connect(remove, &QPushButton::clicked, this, &ListWidget::removeItem);
  connect(seeEvent, &QPushButton::clicked, this, &ListWidget::see);
  connect(AllEvents, &QPushButton::clicked, this, &ListWidget::synhronize);
  
  setLayout(hbox);
}
 
void ListWidget::addItem() 
{    
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

    auto event = Event(1, s_name.toStdString(), s_info.toStdString(), s_address.toStdString(), s_date.toStdString(), s_time.toStdString(), "", std::time(nullptr), 0);
    ClientData::get_instance().data_base.add_event(event);
    
    lw->addItem(s_name);

    int r = lw->count() - 1;
    lw->setCurrentRow(r);
  }
}

void ListWidget::renameItem() 
{
  QListWidgetItem *curitem = lw->currentItem();
  int r = lw->row(curitem);
  
  if (r == -1){return;}
  
  auto q_ev_name = curitem->text();
  auto ev_name = q_ev_name.toStdString();

  auto events =  ClientData::get_instance().data_base.get_all_events();

  QString r_name = QInputDialog::getText(this, "Item", 
      "Enter new item", QLineEdit::Normal, q_ev_name);
  
  QString s_name = r_name.simplified();
  
  if (!s_name.isEmpty()) 
  { 
    QListWidgetItem *item = lw->takeItem(r);
    events[r].set_name(s_name.toStdString());

    delete item;
    lw->insertItem(r, s_name);
    lw->setCurrentRow(r);

    ClientData::get_instance().data_base.update_event(events[r]);
    ClientData::get_instance().update_event(events[r]);
  }
}

void ListWidget::removeItem() 
{  
  QListWidgetItem *curitem = lw->currentItem();
  int r = lw->row(curitem);
  if (r == -1){return;}
  auto q_ev_name = curitem->text();
  auto ev_name = q_ev_name.toStdString();
  auto ev = Event(ev_name, "", "");

  auto events = ClientData::get_instance().data_base.get_all_events();

  QListWidgetItem *item = lw->takeItem(r);
  delete item;

  events[r].set_archived(1);
  ClientData::get_instance().data_base.update_event(events[r]);
  ClientData::get_instance().update_event(events[r]);
}

void ListWidget::see()
{
  QListWidgetItem *curitem = lw->currentItem();
  int r = lw->row(curitem);

  if (r == -1){return;}

  auto q_ev_name = curitem->text();
  auto ev_name = q_ev_name.toStdString();

  auto events = ClientData::get_instance().data_base.get_all_events();
  int id = events[r].get_id();

  auto event =  ClientData::get_instance().data_base.find_event_by_id(id);

  if (event.get_name() != "") 
  { 
    Dialog *dg = new Dialog();
    dg->setEvent(event);
    dg->update();
    dg->show();
  }

  updateEventsList();
}

void ListWidget::updateEventsList()
{
  if (lw->count() != 0) {
    lw->clear();
  }

  auto events = ClientData::get_instance().data_base.get_all_events();

  for(int it = 0; it < events.size(); it++)
  {
    QString c_text = QString::fromStdString(events[it].get_name());
    QString s_text = c_text.simplified();
    
    if (!s_text.isEmpty()) 
    {   
      lw->addItem(s_text);
      int r = lw->count() - 1;
      lw->setCurrentRow(r);
    }
  }
  printf("here\n");
}

void ListWidget::synhronize()
{
  ClientData::get_instance().update_events();
  ClientData::get_instance().data_base.print_all_events();

  updateEventsList();
}

#include <moc_listwidget.cpp>