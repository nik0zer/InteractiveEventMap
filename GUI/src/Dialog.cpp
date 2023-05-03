#include "../include/Dialod.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>

Dialog::Dialog(QDialog* parent) : QDialog(parent)
{
  auto& Base = DataBase::get_instance();


    QBoxLayout* layout = new QHBoxLayout(this);
    

    /*QPushButton* okBtn = new QPushButton( "OK" );
    connect( okBtn, SIGNAL( clicked() ), SLOT( accept() ) );
    layout->addWidget( okBtn );

    QPushButton* applyBtn = new QPushButton( "Apply" );
    connect( applyBtn, SIGNAL( clicked() ), SIGNAL( applied() ) );
    layout->addWidget( applyBtn);*/

    QPushButton* cancelBtn = new QPushButton( "Cancel" );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( reject() ) );
    
    lw = new QListWidget(this);

    layout->addWidget(lw);
    layout->addWidget(cancelBtn);

    setLayout( layout );
}

void Dialog::update()
{
  lw->addItem(QString::fromStdString(event_.get_name()));
  lw->addItem(QString::fromStdString(event_.get_address()));
  lw->addItem(QString::fromStdString(event_.get_time()));
  lw->addItem(QString::fromStdString(event_.get_info()));
  lw->addItem(QString::fromStdString(event_.get_date()));
}

Dialog::~Dialog(){};

#include <moc_Dialod.cpp>