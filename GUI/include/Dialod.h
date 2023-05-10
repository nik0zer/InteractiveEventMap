#ifndef DIALOD_H
#define DIALOD_H

#pragma once 
#include <QDialog>
#include <QApplication>
#include <QWidget>
#include <vector>
#include "client_sql.h"
#include <QListWidget>

class Dialog : public QDialog {
    Q_OBJECT

Event event_;
QListWidget *lw;
public:
    Dialog(QDialog* parent = 0);
    virtual ~Dialog();
    void setEvent(Event event) {event_ = event;};
    void update();
};

#endif