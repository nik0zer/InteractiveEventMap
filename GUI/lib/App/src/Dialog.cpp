#include "Dialod.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>

Dialog::Dialog(QDialog* parent) : QDialog(parent)
{
    QBoxLayout* layout = new QHBoxLayout(this);
    

    /*QPushButton* okBtn = new QPushButton( "OK" );
    connect( okBtn, SIGNAL( clicked() ), SLOT( accept() ) );
    layout->addWidget( okBtn );

    QPushButton* applyBtn = new QPushButton( "Apply" );
    connect( applyBtn, SIGNAL( clicked() ), SIGNAL( applied() ) );
    layout->addWidget( applyBtn);*/

    QPushButton* cancelBtn = new QPushButton( "Cancel" );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( reject() ) );
    layout->addWidget( cancelBtn );

    lw = new QListWidget(this);
  
    for(int it = 0; it < events_.size(); it++)
     {lw->addItem(QString::fromStdString(events_[it].get_name()));} 

    

    setLayout( layout );
}



void Dialog::setEventVector(std::vector<Event> events){events_ = events;};

void Dialog::updateEventsList()
{
  
for(int it = 0; it < events_.size(); it++)
{
  QString c_text = QString::fromStdString(events_[it].get_name());
  QString s_text = c_text.simplified();
  
  if (!s_text.isEmpty()) {
      
    lw->addItem(s_text);
    lw->addItem(QString::fromStdString(events_[it].get_address()));
    int r = lw->count() - 1;
    lw->setCurrentRow(r);
  }
}


  printf("here\n");
  

}

Dialog::~Dialog(){};

#include "moc_Dialod.cpp"