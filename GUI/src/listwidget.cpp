#include "../include/listwidget.h"
#include "../include/Dialod.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QLabel>
 
ListWidget::ListWidget(QWidget *parent)
    : QWidget(parent) {
 
  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->setSpacing(10);
 
  QHBoxLayout *hbox = new QHBoxLayout(this);
 
  lw = new QListWidget(this);
  
  for(int it = 0; it < events_.size(); it++)
  {lw->addItem(QString::fromStdString(events_[it].get_name()));} 
  /*lw->addItem("The Omen"); 
  lw->addItem("The Exorcist");
  lw->addItem("Notes on a scandal");
  lw->addItem("Fargo");
  lw->addItem("Capote");*/
 
  add = new QPushButton("Add event", this);
  rename = new QPushButton("Rename event", this);
  remove = new QPushButton("Remove event", this);
  removeAll = new QPushButton("Remove All", this);
  seeEvent = new QPushButton("See event", this);

  vbox->setSpacing(3);
  vbox->addStretch(1);
  vbox->addWidget(add);
  vbox->addWidget(rename);
  vbox->addWidget(remove);
  vbox->addWidget(removeAll);
  vbox->addWidget(seeEvent);
  vbox->addStretch(1);
 
  hbox->addWidget(lw);
  hbox->addSpacing(15);
  hbox->addLayout(vbox);
  
  connect(add, &QPushButton::clicked, this, &ListWidget::addItem);
  connect(rename, &QPushButton::clicked, this, &ListWidget::renameItem);
  connect(remove, &QPushButton::clicked, this, &ListWidget::removeItem);
  connect(removeAll, &QPushButton::clicked, this, &ListWidget::clearItems);
  connect(seeEvent, &QPushButton::clicked, this, &ListWidget::see);
  
  setLayout(hbox);
}
 
void ListWidget::addItem() {
    
  QString c_text = QInputDialog::getText(this, "Item", "Enter new item");
  QString s_text = c_text.simplified();
  
  if (!s_text.isEmpty()) {
      
    lw->addItem(s_text);
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
    
    if (r != -1) { 
      Dialog *dg = new Dialog();
      dg->setEventVector(events_);
      dg->updateEventsList();
      dg->show();
    
  }

}

void ListWidget::setEventVector(std::vector<Event> events){events_ = events;};

void ListWidget::updateEventsList()
{
  
for(int it = 0; it < events_.size(); it++)
{
  QString c_text = QString::fromStdString(events_[it].get_name());
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