#ifndef CCREATEPRIZEWINDOW_H
#define CCREATEPRIZEWINDOW_H

#include <QMainWindow>

namespace Ui {
class CCreatePrizeWindow;
}

class CCreatePrizeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CCreatePrizeWindow(QWidget *parent = nullptr);
    ~CCreatePrizeWindow();
    QString getDipTemPath();
    QString getSeparator();
    QString getFlaKey();
    QString getBuiFolPath();
    QStringList getStuNameList();
signals:
    void isCreatePrize();

private slots:
    void on_toolButtonTemPath_clicked();

private:
    Ui::CCreatePrizeWindow *ui;
};

#endif // CCREATEPRIZEWINDOW_H
