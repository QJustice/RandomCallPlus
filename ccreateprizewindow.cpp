#include "ccreateprizewindow.h"
#include "ui_ccreateprizewindow.h"

CCreatePrizeWindow::CCreatePrizeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CCreatePrizeWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowModality(Qt::ApplicationModal);

    connect(ui->pushButton, &QPushButton::clicked, this, &CCreatePrizeWindow::isCreatePrize);
}

CCreatePrizeWindow::~CCreatePrizeWindow()
{
    delete ui;
}

QString CCreatePrizeWindow::getDipTemPath()
{
    return ui->lineEditTemPath->text();
}

QString CCreatePrizeWindow::getSeparator()
{
    return ui->lineEditSeparator->text();
}

QString CCreatePrizeWindow::getFlaKey()
{
    return ui->lineEditKey->text();
}

QString CCreatePrizeWindow::getBuiFolPath()
{
    return ui->lineEditBuiPath->text();
}

QStringList CCreatePrizeWindow::getStuNameList()
{
    QString splitStr = this->getSeparator();
    if ("" == splitStr)                         // 默认分隔符号为\n
        splitStr = "\n";
    return ui->textEdit->toPlainText().split(splitStr);
}
