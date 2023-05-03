#ifndef AUTH_H
#define AUTH_H

#pragma once

#include <QWidget>
#include "sql_tools.h"
#include <list>
 
class auth_window : public QWidget
{
    Q_OBJECT
 
public:
    explicit auth_window(QWidget *parent = nullptr);
    virtual ~auth_window();
    QString getLogin();
    QString getPass();

signals:
    void login_button_clicked();
    void register_button_clicked();
 
private slots:
    void on_nameLineEdit_textEdited();
    void on_passwordLineEdit_textEdited();
    void on_loginPushButton_clicked();
    void on_registerPushButton_clicked();
 
private:
    QString m_username;
    QString m_userpass;    
};


#endif
