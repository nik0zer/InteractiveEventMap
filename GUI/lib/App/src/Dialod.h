#pragma once 
#include <QDialog>
#include <QApplication>
#include <QWidget>
#include <vector>
#include "../../Pearsons/pearsons.h"
#include <QListWidget>

class Dialog : public QDialog {
    Q_OBJECT

std::vector<Event> events_;
QListWidget *lw;
public:
    Dialog( QDialog* parent = 0 );
    virtual ~Dialog();
    void setEventVector(std::vector<Event> events);
    void updateEventsList();
};