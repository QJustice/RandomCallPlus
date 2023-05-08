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
    void setPushButtonText(QString _text);
    void setTableHead(QStringList _tabelHeads);                          // 设置表头
    void setTabelItem(int _row, int _column, QString _item);
    void setTabelColRowCount(int _row, int _column);
    void setTableDataFromExcle(QString filePath);
    void setStuDataList(QList<std::pair<QString, QString>>* _stuDataList);
    void saveStuData(QString floderPath);
    std::map<QString, QStringList>* getStuData();

signals:
    void pushButtoncliked();

public slots:
    void getFilePath();
    void getFolderPath();
    void pushButtonCliked();
private:
    Ui::CStudentDataList *ui;
    QList<QTableWidgetItem*> *m_pTableItemList = nullptr;
    std::map<QString, QStringList> *m_stuDataMap = nullptr;
    QList<std::pair<QString, QString>> *m_stuDataList = nullptr;
};

#endif // CSTUDENTDATALIST_H
