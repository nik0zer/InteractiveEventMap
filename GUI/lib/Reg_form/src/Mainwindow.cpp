#include "Mainwindow.h"
//#include "ui_mainwindow.h"
#include "auth_window.h"
#include "reg_window.h"
#include <QtDebug>
#include <QtSql/QtSql>
#include "../../App/src/skeleton.h"


MainWindow::~MainWindow(){};

MainWindow::MainWindow(QWidget *parent) :                                               //реализация конструктора главного окна
    QMainWindow(parent)                                                              //список инициализаци
{
    ////////////////////////////////////////////
    // создаем main UI
    //////////////////////////////////////////////
    auth_window window;
 
    window.setWindowTitle("Auth_window");
      

    user_counter = 0;
    m_loginSuccesfull = false;
    connect(&ui_Auth, SIGNAL(login_button_clicked()),  this, SLOT(authorizeUser()));  //соединение сигнала кнопки авторизации экземпляра окна авторизации
                                                                                       //со слотом-обработчиком авторизации

    connect(&ui_Auth,SIGNAL(destroyed()),this, SLOT(show()));                           //соединение сигнала уничтожения экземпляра окна авторизации
                                                                                           //с методом отображения главного окна

    connect(&ui_Auth,SIGNAL(register_button_clicked()), this,SLOT(registerWindowShow())); //соединение сигнала кнопки регистрации экземпляра окна авторизации
                                                                                          //со слотом вызывающим окно регистрации

    connect(&ui_Reg,SIGNAL(register_button_clicked2()),this,SLOT(registerUser()));       //соединение кнопки регистрации экземпляра окна авторизации
                                                                                        //со слотом-обработчиком регистраци
    connect(&ui_Reg,SIGNAL(destroyed()), &ui_Auth, SLOT(show()));

                                                                //установка интерфейса главного окна

      
    connect(&ui_Auth,SIGNAL(destroyed()), this, SLOT(AuthAquired()));
}
/////////////////////////
// НУЖНО РЕАЛИЗОВАТЬ
///////////////////////
void MainWindow::authorizeUser() 
{
   /* m_username          = ui_Auth.getLogin();
    m_userpass          = ui_Auth.getPass();

    QString str_t       = " SELECT * "
                          " FROM userlist "
                          " WHERE name = '%1'";
    //int db_number       = 0;

    QString username    = "";

    QString userpass    = "";

    int xPos            = 0;

    int yPos            = 0;

    int width           = 0;

    int length          = 0;

    db_input    = str_t.arg(m_username);

    QSqlQuery query;

    QSqlRecord rec;

    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }
    rec = query.record();
    query.next();
    user_counter   = query.value(rec.indexOf("number")).toInt();
    username    = query.value(rec.indexOf("name")).toString();
    userpass    = query.value(rec.indexOf("pass")).toString();
    if(m_username != username || m_userpass != userpass)
    {
        qDebug() << "Password missmatch" << username << " " << userpass;
        m_loginSuccesfull = false;
    }
    else
    {
        m_loginSuccesfull = true;
        xPos    = query.value(rec.indexOf("xpos")).toInt();
        yPos    = query.value(rec.indexOf("ypos")).toInt();
        width   = query.value(rec.indexOf("width")).toInt();
        length  = query.value(rec.indexOf("length")).toInt();
        ui_Auth.close();
        ui_Reg.close();
        this->setGeometry(xPos,yPos,width, length);
        this->show();
    }*/

    printf("You are in authorized users))\n");
    ui_Auth.hide();
    ui_App.show();

    
}

void MainWindow::registerWindowShow()
{
    ui_Auth.hide();
    ui_Reg.show();
}

void MainWindow::registerUser()
{
    if(ui_Reg.checkPass())
    {
       /* QSqlQuery query;
        QSqlRecord rec;
        QString str_t       = "SELECT COUNT(*) "
                              "FROM userlist;";
        db_input            = str_t;
        if(!query.exec(db_input))
        {
            qDebug() << "Unable to get number " << query.lastError() << " : " << query.lastQuery();
            return;
        }
        else
        {
            query.next();
            rec = query.record();
            user_counter = rec.value(0).toInt();
            qDebug() << user_counter;
        }


        m_username          = ui_Reg.getName();
        m_userpass          = ui_Reg.getPass();
        user_counter++;
        str_t               =   "INSERT INTO userlist(number, name, pass, xpos, ypos, width, length)"
                                "VALUES(%1, '%2', '%3', %4, %5, %6, %7);";
        db_input            = str_t .arg(user_counter)
                                    .arg(m_username)
                                    .arg(m_userpass)
                                    .arg(0)
                                    .arg(0)
                                    .arg(800)
                                    .arg(400);



        if(!query.exec(db_input))
        {
            qDebug() << "Unable to insert data"  << query.lastError() << " : " << query.lastQuery();
        }
        else
        {
            ui_Reg.hide();
            ui_Auth.show();
        }
    }
    else
    {
        qDebug() << "Confirm password coorectly";
    }*/

    }
}


void MainWindow::display()                                                              //реализация пользотвальского метода отображения главного окна
{
    ui_Auth.show();                                                                     //отобразить окно авторизации(НЕ главное окно)
}




/*void MainWindow::wipeDB()
{
    mw_db.removeDatabase("authorisation");
}*/