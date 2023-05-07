#include "cstudentdatalist.h"
#include "ui_cstudentdatalist.h"
#include <xlsxdocument.h>
#include <xlsxcellrange.h>
#include <QFileDialog>

CStudentDataList::CStudentDataList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CStudentDataList)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->m_pTableItemList = new QList<QTableWidgetItem*>;
    // 表格宽度自动根据UI进行计算
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->toolButtonFilePath, &QToolButton::clicked, this, &CStudentDataList::getFilePath);
    connect(ui->pushButton, &QPushButton::clicked, this, &CStudentDataList::importData);
}

CStudentDataList::~CStudentDataList()
{
    delete ui;
    for (int i = 0; i < this->m_pTableItemList->size(); i++)
    {
        delete this->m_pTableItemList->at(i);
        this->m_pTableItemList->removeAt(i);
    }
    if (this->m_stuDataList)
        delete this->m_stuDataList;
}

void CStudentDataList::setTableHead(QStringList _tabelHeads)
{
    ui->tableWidget->setHorizontalHeaderLabels(_tabelHeads);
}

void CStudentDataList::setTabelItem(int _row, int _column, QString _item)
{

    QTableWidgetItem *tempItem = new QTableWidgetItem(_item);
    this->m_pTableItemList->append(tempItem);

    ui->tableWidget->setItem(_row, _column, tempItem);
}

void CStudentDataList::setTabelColRowCount(int _row, int _column)
{
    ui->tableWidget->setColumnCount(_column);
    ui->tableWidget->setRowCount(_row);
}

void CStudentDataList::setTableDataFromExcle(QString filePath)
{
    QStringList table_h_headers;                    // 保存表头
    this->m_stuDataList = new std::map<QString, QStringList>;
    QXlsx::Document xlsx(filePath);
    xlsx.selectSheet(xlsx.sheetNames()[0]);
    QXlsx::CellRange range = xlsx.dimension();

    this->setTabelColRowCount(range.rowCount() - 1, range.columnCount());


    for(int i = 1; i <= range.rowCount(); i++)
        for (int j = 1; j <=  range.columnCount(); j++)
        {
            if(xlsx.cellAt(i,j)) //若单元格没有内容返回空指针，忽略容易崩溃
            {
                if (1 == i)
                {
                    table_h_headers.append(xlsx.cellAt(i,j)->value().toString());
                    continue;
                }
                else if (2 == i)
                {
                    this->m_stuDataList->insert(std::make_pair("HEAD", table_h_headers));
                    this->setTableHead(table_h_headers);
                }
                if (table_h_headers.indexOf("学号") == j - 1)
                    this->m_stuDataList->insert(std::make_pair(xlsx.cellAt(i, j)->value().toString(), QStringList()));
                else
                    this->m_stuDataList->at(xlsx.cellAt(i, table_h_headers.indexOf("学号") + 1)->value().toString()).append(xlsx.cellAt(i,j)->value().toString());
                // QTable 的第一行从0的位置开始
                this->setTabelItem(i - 2, j - 1, xlsx.cellAt(i, j)->value().toString());
            }
            else
                this->setTabelItem(i - 2, j - 1, "NULL");
        }

    qDebug() << *(this->m_stuDataList);
}

void CStudentDataList::saveStuData(QString floderPath)
{
    QXlsx::Document xlsx;
    for(int i = 0;i < ui->tableWidget->rowCount(); i++)
    {
        for(int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            if(ui->tableWidget->item(i, j) != NULL)
            {               //一定要先判断非空，否则会报
                xlsx.write(i + 1, j + 1, ui->tableWidget->item(i, j)->text());
            }
            else
                xlsx.write(i + 1, j + 1, "NULL");
        }
    }
    xlsx.saveAs("student_data.xlsx");
}

std::map<QString, QStringList>* CStudentDataList::getStuData()
{
//    for(int i = 0;i < ui->tableWidget->rowCount(); i++)
//    {
//        for(int j = 0; j < ui->tableWidget->columnCount(); j++)
//        {
//            if(ui->tableWidget->item(i, j) != NULL)
//            {               //一定要先判断非空，否则会报
//                qDebug() <<  ui->tableWidget->item(i, j)->text();
//            }
//            else
//                qDebug() <<  "NULL";
//        }
//    }
    std::map<QString, QStringList>* tempPtr = this->m_stuDataList;
    this->m_stuDataList = nullptr;
    return tempPtr;
}

void CStudentDataList::getFilePath()
{
    ui->lineEditFilePath->setText(QFileDialog::getOpenFileName(this));
    this->setTableDataFromExcle(ui->lineEditFilePath->text());
}

void CStudentDataList::importData()
{
    emit this->importDatacliked();
}
