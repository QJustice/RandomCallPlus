#ifndef CCREATECLASSWINDOW_H
#define CCREATECLASSWINDOW_H

#include <QMainWindow>

namespace Ui {
class CCreateClassWindow;
}

class CCreateClassWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CCreateClassWindow(QWidget *parent = nullptr);
    ~CCreateClassWindow();
    QString getClassNumber();
    QString getClassName();

signals:
    void isCreCla();

private slots:

    void creClaBtn();

private:
    Ui::CCreateClassWindow *ui;
};

#endif // CCREATECLASSWINDOW_H
