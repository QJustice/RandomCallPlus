#ifndef CSTATISTICSWINDOW_H
#define CSTATISTICSWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>

namespace Ui {
class CStatisticsWindow;
}

class CStatisticsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CStatisticsWindow(QWidget *parent = nullptr);
    ~CStatisticsWindow();
    void setTableHead(QStringList _tabelHeads);                          // 设置表头
    void setTabelItem(int _row, int _column, QString _item);
    void setTabelColRowCount(int _row, int _column);


private:
    Ui::CStatisticsWindow *ui;
    QList<QTableWidgetItem*> *m_pTableItemList;
};

#endif // CSTATISTICSWINDOW_H
