#ifndef CCHANGECLASSWINDOW_H
#define CCHANGECLASSWINDOW_H

#include <QMainWindow>

namespace Ui {
class CChangeClassWindow;
}

class CChangeClassWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CChangeClassWindow(QWidget *parent = nullptr);
    ~CChangeClassWindow();
    void setClassList(QStringList _strLis);
    QString getNowChange();
signals:
    void isChaCla();
private slots:

    void chaClaComBoxItem();
//    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::CChangeClassWindow *ui;
};

#endif // CCHANGECLASSWINDOW_H
