#include <QMainWindow>
#include <QString>
#include <QtSql/QtSql>
#include "auth_window.h"
#include "reg_window.h"
#include "../../App/src/skeleton.h"
#include "../../App/src/listwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
 
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
    void display(); // прототип пользовательской функции отображения
    bool connectDB(); // прототип метода подключения к БД
 
private:
    
    auth_window ui_Auth; // экземпляры окна авторизации и окна регистрации
    reg_window ui_Reg; // принадлежат главному окну
    Skeleton   ui_App;

    
    QString m_username; // строки для обработки 
    QString m_userpass; // пользовательского ввода
    
    /*QString db_input; // строка для отправки запроса к БД
    
    QSqlDatabase mw_db; // экземпляр подключения к БД*/
    
    int user_counter; // счетчик пользователей
    bool m_loginSuccesfull; // флаг успешной авторизации
private slots:
    void authorizeUser(); // пользовательские слоты
    void registerWindowShow();
    void registerUser();
};