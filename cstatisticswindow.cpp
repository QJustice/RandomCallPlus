#include "cstatisticswindow.h"
#include "ui_cstatisticswindow.h"
#include "config.h"
#include <QDebug>

CStatisticsWindow::CStatisticsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CStatisticsWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->m_pTableItemList = new QList<QTableWidgetItem*>;
    // 表格宽度自动根据UI进行计算
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

CStatisticsWindow::~CStatisticsWindow()
{
    delete ui;
    for (int i = 0; i < this->m_pTableItemList->size(); i++)
    {
        delete this->m_pTableItemList->at(i);
        this->m_pTableItemList->removeAt(i);
    }
    qDebug() << this->m_pTableItemList->size();
}

void CStatisticsWindow::setTableHead(QStringList _tabelHeads)
{
    ui->tableWidget->setHorizontalHeaderLabels(_tabelHeads);
}

void CStatisticsWindow::setTabelItem(int _row, int _column, QString _item)
{

    QTableWidgetItem *tempItem = new QTableWidgetItem(_item);
    this->m_pTableItemList->append(tempItem);

    ui->tableWidget->setItem(_row, _column, tempItem);
}

void CStatisticsWindow::setTabelColRowCount(int _row, int _column)
{
    ui->tableWidget->setColumnCount(_column);
    ui->tableWidget->setRowCount(_row);
}


