#include "Mainwindow.h"
#include "auth_window.h"
#include "reg_window.h"
#include <QtDebug>
#include <QString>
#include <QtSql/QtSql>
#include "../../App/src/skeleton.h"
#include "../../Pearsons/pearsons.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>


MainWindow::~MainWindow(){};

MainWindow::MainWindow(QWidget *parent) :                                               //реализация конструктора главного окна
    QMainWindow(parent)                                                               //список инициализаци
{
    ////////////////////////////////////////////
    // создаем main UI
    //////////////////////////////////////////////
    auth_window window;
    
    
 
    window.setWindowTitle("Auth_window");
      

    /*user_counter = 0;
    m_loginSuccesfull = false;*/
    connect(&ui_Auth, SIGNAL(login_button_clicked()),  this, SLOT(authorizeUser()));  //соединение сигнала кнопки авторизации экземпляра окна авторизации
                                                                                       //со слотом-обработчиком авторизации

    connect(&ui_Auth,SIGNAL(destroyed()),this, SLOT(show()));                           //соединение сигнала уничтожения экземпляра окна авторизации
                                                                                           //с методом отображения главного окна

    connect(&ui_Auth,SIGNAL(register_button_clicked()), this,SLOT(registerWindowShow())); //соединение сигнала кнопки регистрации экземпляра окна авторизации
                                                                                          //со слотом вызывающим окно регистрации

    connect(&ui_Reg,SIGNAL(register_button_clicked2()),this,SLOT(registerUser()));       //соединение кнопки регистрации экземпляра окна авторизации
                                                                                        //со слотом-обработчиком регистраци
    connect(&ui_Reg,SIGNAL(destroyed()), &ui_Auth, SLOT(show()));
 
   // connect(&ui_Auth,SIGNAL(destroyed()), this, SLOT(AuthAquired()));
}

void MainWindow::authorizeUser() // работает
{
    std::vector<Person> pers = getPersonVec();

    //if (Database::get_instance().person_verify(person));

    for (int it = 0; it < pers.size(); it++)
    {
        if (QString::fromStdString(pers[it].get_login()) == ui_Auth.getLogin())
        {
            if (QString::fromStdString(pers[it].get_password()) == ui_Auth.getPass())
            {
                printf("You are in authorized users))\n");
                ui_Auth.hide();
                updateEventVec();
                ui_App.show();
            }

            else{printf("bad password\n");}
        }

        else {printf("please go to register window");}
    }
}

void MainWindow::registerWindowShow()
{
    ui_Auth.hide();
    ui_Reg.show();
}

void MainWindow::registerUser()
{
    auto pers = getPersonVec();
    for (int it = 0; it < pers.size(); it++)
    {
        if (QString::fromStdString(pers[it].get_login()) == ui_Reg.getName())
        {
            printf("логин занят\n");
            return;
        } 
        else{continue;}
    }

    if(ui_Reg.checkPass())
    {
       printf("you are registered\n");
       ui_Reg.hide();
       updateEventVec();
       ui_App.show();
    }

    else{printf("пароли не совпадают!\n");}
}


void MainWindow::display()                                                              //реализация пользотвальского метода отображения главного окна
{
    ui_Auth.show();                                                                     //отобразить окно авторизации(НЕ главное окно)
}

void MainWindow::updateEventVec()
{
    ui_App.setEventVector(events_);
    ui_App.updateEventsList();
}


