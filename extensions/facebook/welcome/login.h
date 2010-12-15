#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
    class Login;
}

class Login : public QWidget {
    Q_OBJECT
public:
    Login(QWidget *parent = 0);
    ~Login();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
