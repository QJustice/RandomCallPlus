 #ifndef CADDSTUDENTWINDOW_H
#define CADDSTUDENTWINDOW_H

#include <QMainWindow>

namespace Ui {
class CAddStudentWindow;
}

class CAddStudentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CAddStudentWindow(QWidget *parent = nullptr);
    ~CAddStudentWindow();
    QString getStudentNumber();
    QString getStudentsName();

signals:
    void isAddStu();

private:
    Ui::CAddStudentWindow *ui;
};

#endif // CADDSTUDENTWINDOW_H
