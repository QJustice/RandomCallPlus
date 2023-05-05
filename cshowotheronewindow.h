#ifndef CSHOWOTHERONEWINDOW_H
#define CSHOWOTHERONEWINDOW_H

#include <QMainWindow>

namespace Ui {
class CShowOtherOneWindow;
}

class CShowOtherOneWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CShowOtherOneWindow(QWidget *parent = nullptr);
    ~CShowOtherOneWindow();
    void setlist(QList<std::pair<QString, QString>> strlis1, QList<std::pair<QString, QString>> strlis2);

private:
    Ui::CShowOtherOneWindow *ui;
};

#endif // CSHOWOTHERONEWINDOW_H
