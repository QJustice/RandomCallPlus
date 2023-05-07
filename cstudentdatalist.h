#ifndef CSTUDENTDATALIST_H
#define CSTUDENTDATALIST_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <map>

namespace Ui {
class CStudentDataList;
}

class CStudentDataList : public QMainWindow
{
    Q_OBJECT

public:
    explicit CStudentDataList(QWidget *parent = nullptr);
    ~CStudentDataList();
    void setTableHead(QStringList _tabelHeads);                          // 设置表头
    void setTabelItem(int _row, int _column, QString _item);
    void setTabelColRowCount(int _row, int _column);
    void setTableDataFromExcle(QString filePath);
    void saveStuData(QString floderPath);
    std::map<QString, QStringList>* getStuData();
signals:
    void importDatacliked();

public slots:
    void getFilePath();
    void importData();
private:
    Ui::CStudentDataList *ui;
    QList<QTableWidgetItem*> *m_pTableItemList;
    std::map<QString, QStringList> *m_stuDataList;
};

#endif // CSTUDENTDATALIST_H
