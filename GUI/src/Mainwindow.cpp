#include "../include/Mainwindow.h"
#include "../include/auth_window.h"
#include "../include/reg_window.h"
#include <QtDebug>
#include <QString>
#include <QtSql/QtSql>
#include "../include/skeleton.h"
#include "client_sql.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <iostream>


MainWindow::~MainWindow(){};

MainWindow::MainWindow(QWidget *parent) :                                              
    QMainWindow(parent)                                                               
{
    auth_window window;
    
    window.setWindowTitle("Auth_window");
      
    connect(&ui_Auth, SIGNAL(login_button_clicked()),  this, SLOT(authorizeUser()));                                                                                     
    connect(&ui_Auth,SIGNAL(destroyed()),this, SLOT(show()));                          
    connect(&ui_Auth,SIGNAL(register_button_clicked()), this,SLOT(registerWindowShow())); 
    connect(&ui_Reg,SIGNAL(register_button_clicked2()),this,SLOT(registerUser()));                                                                                          
    connect(&ui_Reg,SIGNAL(destroyed()), &ui_Auth, SLOT(show()));
}

void MainWindow::authorizeUser() 
{
    QString name = ui_Auth.getLogin();
    QString password = ui_Auth.getPass();

    auto per = User(name.toStdString(), password.toStdString());
    ClientData::get_instance().set_new_user(per);

    printf("here\n");
    if (ClientData::get_instance().verify_user())
    {
        printf("here2\n");
        ui_Auth.hide();
        ui_App.show();
    }
    else
    {
        std::cout << "You are not registered" << std::endl;
    }
}

void MainWindow::registerWindowShow()
{
    ui_Auth.hide();
    ui_Reg.show();
}

void MainWindow::registerUser()
{
    QString name = ui_Reg.getName();
    QString password = ui_Reg.getPass(); 

    auto per = User(name.toStdString(), password.toStdString());
    ClientData::get_instance().set_new_user(per);
    printf("here\n");

    if (!ClientData::get_instance().verify_user())
    {
        printf("here2\n");
        if(ui_Reg.checkPass())
        {
            ClientData::get_instance().registration(per);
            ui_Reg.hide();
            ui_App.show();
            printf(">>>>>>>>>>>>> you are registered <<<<<<<<<<<<< \n");
        }  
        else 
        {
            printf(">>>>>>>>>>>>>> wrong password <<<<<<<<<<<<<<<< \n");
        } 
    }
}

void MainWindow::display()                                                              
{
    ui_Auth.show();                                                                     
}

#include <moc_Mainwindow.cpp>