#include "../include/auth_window.h"
#include <QtDebug>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <iostream>
#include <QTextStream>


auth_window::~auth_window(){};

auth_window::auth_window(QWidget *parent) :                               
    QWidget(parent)                                                          
{
    QGridLayout *grid = new QGridLayout(); 
    grid->setSpacing(2);                 

    QLabel *welcome = new QLabel(QString("Authentificate yourself"));
    QLabel *name = new QLabel(QString("Name:"));
    QLabel *password = new QLabel(QString("Password:"));

    QLineEdit *nameLineEdit = new QLineEdit();
    QLineEdit *passwordLineEdit = new QLineEdit();
    
    QPushButton* log = new QPushButton("Log In", this);
    QPushButton* reg = new QPushButton("Register", this);

    grid->addWidget(welcome, 1 , 2, 1 ,1);

    grid->addWidget(name, 2, 1, 1 ,1);
    grid->addWidget(nameLineEdit, 2, 2, 1, 1);
    grid->addWidget(password, 3, 1, 1 ,1);
    grid->addWidget(passwordLineEdit, 3, 2, 1, 1);
    
    grid->addWidget(log, 4, 2, 1, 1);
    grid->addWidget(reg, 5, 2, 1, 1);
    
    connect(reg, &QPushButton::clicked, this, &auth_window::on_registerPushButton_clicked);
    connect(log, &QPushButton::clicked, this, &auth_window::on_loginPushButton_clicked);
    connect(nameLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(on_nameLineEdit_textEdited()));
    connect(passwordLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(on_passwordLineEdit_textEdited()));
    
    setLayout(grid);                                                      
}

void auth_window::on_registerPushButton_clicked()                         
{
    emit register_button_clicked();
}

void auth_window::on_loginPushButton_clicked()                              
{
    emit login_button_clicked();
}

void auth_window::on_nameLineEdit_textEdited()
{
    QLineEdit *clickedLine = qobject_cast<QLineEdit*>(sender());
    auth_window::m_username = clickedLine->text();
}

void auth_window::on_passwordLineEdit_textEdited()
{
    QLineEdit *clickedLine = qobject_cast<QLineEdit*>(sender());
    auth_window::m_userpass = clickedLine->text();
}

QString auth_window::getLogin()
{
    return auth_window::m_username;
}

QString auth_window::getPass()
{
    return auth_window::m_userpass;
}

#include <moc_auth_window.cpp>