#ifndef CDELETECLASSWINDOW_H
#define CDELETECLASSWINDOW_H

#include <QMainWindow>

namespace Ui {
class CDeleteClassWindow;
}

class CDeleteClassWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CDeleteClassWindow(QWidget *parent = nullptr);
    ~CDeleteClassWindow();
    QString getClassNumber();
    QString getClassName();
    void setClassName(QString _className);

signals:
    void isDelCla();
    void claNumChange();

private slots:

    void delClaBtn();

private:
    Ui::CDeleteClassWindow *ui;
};

#endif // CDELETECLASSWINDOW_H
