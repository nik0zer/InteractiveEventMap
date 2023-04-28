#ifndef REG_WINDOW_H
#define REG_WINDOW_H

#include <QWidget>
 
class reg_window : public QWidget
{
    Q_OBJECT
 
public:
    explicit reg_window(QWidget *parent = nullptr);
    virtual ~reg_window();
    QString getName();
    QString getPass();
    bool checkPass();
 
signals:
    void register_button_clicked();
 
private slots:
    void on_nameLineEdit_textEdited();
    void on_passwordLineEdit_textEdited();
    void on_confirmLineEdit_textEdited();
    void on_registerPushButton_clicked();
 
private:
    QString m_userName;
    QString m_userPass;
    QString m_confirmation;
};

#endif