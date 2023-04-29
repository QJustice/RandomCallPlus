#ifndef CDELETESTUDENTWINDOW_H
#define CDELETESTUDENTWINDOW_H

#include <QMainWindow>

namespace Ui {
class CDeleteStudentWindow;
}

class CDeleteStudentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CDeleteStudentWindow(QWidget *parent = nullptr);
    ~CDeleteStudentWindow();
    QString getStudentNumber();
    QString getStudentsName();
    void setStudentName(QString _studentName);

signals:
    void isDelStu();
    void stuNumChange();

private:
    Ui::CDeleteStudentWindow *ui;
};

#endif // CDELETESTUDENTWINDOW_H
