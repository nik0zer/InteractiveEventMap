#include "../include/reg_window.h"
#include <QtDebug>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <iostream>
#include <QTextStream>

reg_window::~reg_window(){};

reg_window::reg_window(QWidget *parent) :                               //реализация конструктора окна регистрации
    QWidget(parent)                                                    //список инициализации
{
    QGridLayout *grid = new QGridLayout(); 
    grid->setSpacing(2);                 

    QLabel *welcome = new QLabel(QString("Register yourself"));
    QLabel *name = new QLabel(QString("Name:"));
    QLabel *password = new QLabel(QString("Password:"));
    QLabel *confirm = new QLabel(QString("Confirm:"));

    QLineEdit *nameLineEdit = new QLineEdit();
    QLineEdit *passwordLineEdit = new QLineEdit();
    QLineEdit *confirmLineEdit = new QLineEdit();
    
    
    
    
    QPushButton* reg = new QPushButton("Register", this);

    grid->addWidget(welcome, 1 , 2, 1 ,1);

    grid->addWidget(name, 2, 1, 1 ,1);
    grid->addWidget(nameLineEdit, 2, 2, 1, 1);
    grid->addWidget(password, 3, 1, 1 ,1);
    grid->addWidget(passwordLineEdit, 3, 2, 1, 1);
    grid->addWidget(confirm,  4, 1, 1 ,1);
    grid->addWidget(confirmLineEdit, 4, 2, 1, 1);
    grid->addWidget(reg, 5, 2, 1, 1);
    
    connect(reg, &QPushButton::clicked, this, &reg_window::on_registerPushButton_clicked2);
    connect(nameLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(on_nameLineEdit_textEdited()));
    connect(passwordLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(on_passwordLineEdit_textEdited()));
    connect(confirmLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(on_confirmLineEdit_textEdited()));

    setLayout(grid);
}



void reg_window::on_registerPushButton_clicked2()                        //реализация слота нажатия кнопки регистрации
{
    emit register_button_clicked2(); // нужно будет реализовать
}

void reg_window::on_nameLineEdit_textEdited()
{
    
    QLineEdit *clickedLine = qobject_cast<QLineEdit*>(sender());
    reg_window::m_userName = clickedLine->text();

    //распечатка в строку
    //QTextStream out(stdout); 
    //out << m_userName;
}

void reg_window::on_passwordLineEdit_textEdited()
{
    QLineEdit *clickedLine = qobject_cast<QLineEdit*>(sender());
    reg_window::m_userPass = clickedLine->text();
    //QTextStream out(stdout); 
    //out << m_userPass;
}

void reg_window::on_confirmLineEdit_textEdited()
{
    QLineEdit *clickedLine = qobject_cast<QLineEdit*>(sender());
    reg_window::m_confirmation = clickedLine->text();
    //QTextStream out(stdout); 
    //out << m_confirmation;
}

QString reg_window::getName()
{
    return m_userName;
}

QString reg_window::getPass()
{
    return m_userPass;
}

bool reg_window::checkPass()
{
    return (m_confirmation == m_userPass);
}

#include <moc_reg_window.cpp>