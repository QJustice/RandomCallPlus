#include "cstudentdatalist.h"
#include "ui_cstudentdatalist.h"
#include <xlsxdocument.h>
#include <xlsxcellrange.h>
#include <QFileDialog>
#include <QMessageBox>

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
    connect(ui->pushButton, &QPushButton::clicked, this, &CStudentDataList::pushButtonCliked);
}

CStudentDataList::~CStudentDataList()
{
    delete ui;
    if (NULL != this->m_pTableItemList)
        for (int i = 0; i < this->m_pTableItemList->size(); i++)
        {
            delete this->m_pTableItemList->at(i);
            this->m_pTableItemList->removeAt(i);
        }
    if (this->m_stuDataMap)
    {
        delete this->m_stuDataMap;
    }
}

void CStudentDataList::setPushButtonText(QString _text)
{
    ui->pushButton->setText(_text);
    if ("导出" == _text)
    {
        disconnect(ui->toolButtonFilePath, &QToolButton::clicked, this, &CStudentDataList::getFilePath);
        connect(ui->toolButtonFilePath, &QToolButton::clicked, this, &CStudentDataList::getFolderPath);
    }
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
    this->m_stuDataMap = new std::map<QString, QStringList>;
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
                    this->m_stuDataMap->insert(std::make_pair("HEAD", table_h_headers));
                    this->setTableHead(table_h_headers);
                }
                if (table_h_headers.indexOf("学号") == j - 1)
                    this->m_stuDataMap->insert(std::make_pair(xlsx.cellAt(i, j)->value().toString(), QStringList()));
                else
                    this->m_stuDataMap->at(xlsx.cellAt(i, table_h_headers.indexOf("学号") + 1)->value().toString()).append(xlsx.cellAt(i,j)->value().toString());
                // QTable 的第一行从0的位置开始
                this->setTabelItem(i - 2, j - 1, xlsx.cellAt(i, j)->value().toString());
            }
            else
                this->setTabelItem(i - 2, j - 1, "NULL");
        }
}

void CStudentDataList::setStuDataList(QList<std::pair<QString, QString>>* _stuDataList)
{
    this->m_stuDataList = _stuDataList;
    QStringList table_h_headers;
    table_h_headers << "学号" << "姓名";

    this->setTabelColRowCount(this->m_stuDataList->size(), table_h_headers.size());
    this->setTableHead(table_h_headers);
    for (int i = 0; i < this->m_stuDataList->size(); i++)
    {
        // QTable 的第一行从0的位置开始
        if ("" != m_stuDataList->at(i).first)
            this->setTabelItem(i, 0, m_stuDataList->at(i).first);
        else
            this->setTabelItem(i, 0, "NULL");
        if ("" != m_stuDataList->at(i).first)
            this->setTabelItem(i, 1, m_stuDataList->at(i).second);
        else
            this->setTabelItem(i, 1, "NULL");
    }
    delete m_stuDataList;
}

void CStudentDataList::saveStuData(QString floderPath)
{
    QXlsx::Document xlsx;
    xlsx.write(1, 1, "学号");
    xlsx.write(1, 2, "姓名");
    for(int i = 0;i < ui->tableWidget->rowCount(); i++)
    {
        for(int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            if(ui->tableWidget->item(i, j) != NULL)
            {               //一定要先判断非空，否则会报
                xlsx.write(i + 2, j + 1, ui->tableWidget->item(i, j)->text());
            }
            else
                xlsx.write(i + 2, j + 1, "NULL");
        }
    }
    xlsx.saveAs(QString("%1/student_data.xlsx").arg(floderPath));
    QMessageBox::information(this, "提示", "导出成功");
}

std::map<QString, QStringList>* CStudentDataList::getStuData()
{
    std::map<QString, QStringList>* tempPtr = this->m_stuDataMap;
    this->m_stuDataMap = nullptr;
    return tempPtr;
}

void CStudentDataList::getFilePath()
{
    ui->lineEditFilePath->setText(QFileDialog::getOpenFileName(this));
    if ("" != ui->lineEditFilePath->text() && ui->lineEditFilePath->text().contains(".xlsx"))
        this->setTableDataFromExcle(ui->lineEditFilePath->text());
    else
        qDebug() << "filepath is null";
}

void CStudentDataList::getFolderPath()
{
    ui->lineEditFilePath->setText(QFileDialog::getExistingDirectory(this));
}

void CStudentDataList::pushButtonCliked()
{
    if ("导出" == ui->pushButton->text())
    {
        if ("" != ui->lineEditFilePath->text())
            this->saveStuData(ui->lineEditFilePath->text());
        else
            QMessageBox::critical(this, "error", "路径不可为空");
    }
    else
    {
        if ("" == ui->lineEditFilePath->text())
        {
            QMessageBox::critical(this, "error", "路径不可为空");
            return;
        }
        if (!ui->lineEditFilePath->text().contains(".xlsx"))
        {
            QMessageBox::critical(this, "error", "只可选取.xlsx文件");
            return;
        }
        emit this->pushButtoncliked();
    }
}
