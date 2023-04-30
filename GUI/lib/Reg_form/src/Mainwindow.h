#include <QMainWindow>
#include <QString>
#include <QtSql/QtSql>
#include "auth_window.h"
#include "reg_window.h"
#include "../../App/src/skeleton.h"
#include "../../App/src/listwidget.h"
#include <vector>


class MainWindow : public QMainWindow
{
    Q_OBJECT
 
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    void display(); // прототип пользовательской функции отображения
    bool connectDB(); // прототип метода подключения к БД
    void setPersonVec(std::vector<Person> pers){pers_ = pers;};
    std::vector<Person> getPersonVec() {return pers_;};
    int getPersonVecLen() {return pers_.size();};
    void updateEventVec(); 
    void setEventVec(std::vector<Event> events){events_ = events;};
    
 
private:
    
    auth_window ui_Auth; // экземпляры окна авторизации и окна регистрации
    reg_window ui_Reg; // принадлежат главному окну
    Skeleton   ui_App;

    
    QString m_username; // строки для обработки 
    QString m_userpass; // пользовательского ввода
    std::vector<Person> pers_;
    std::vector<Event> events_;
    
    int user_counter; // счетчик пользователей
    bool m_loginSuccesfull; // флаг успешной авторизации
private slots:
    void authorizeUser(); // пользовательские слоты
    void registerWindowShow();
    void registerUser();
};