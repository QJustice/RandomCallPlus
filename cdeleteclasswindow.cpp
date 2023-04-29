#include "cdeleteclasswindow.h"
#include "ui_cdeleteclasswindow.h"
#include "config.h"

#include <QLineEdit>

CDeleteClassWindow::CDeleteClassWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CDeleteClassWindow)
{
    ui->setupUi(this);
    // 禁用最大化按钮和禁止调节窗口大小
    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(DELETE_CLASS_WINDOW_WIDE, DELETE_CLASS_WINDOW_HIGH);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowModality(Qt::ApplicationModal);
    connect(ui->delClaBtn, &QPushButton::clicked, this, &CDeleteClassWindow::delClaBtn);
    connect(ui->actionDelete, &QAction::triggered, this, &CDeleteClassWindow::delClaBtn);
    connect(ui->actionExit, &QAction::triggered, this, &CDeleteClassWindow::close);
    connect(ui->classNumEdi, &QLineEdit::textEdited, this, &CDeleteClassWindow::claNumChange);
}

CDeleteClassWindow::~CDeleteClassWindow()
{
    delete ui;
}

QString CDeleteClassWindow::getClassNumber()
{
    return ui->classNumEdi->text();
}

QString CDeleteClassWindow::getClassName()
{
    return ui->classNamEdi->text();
}

void CDeleteClassWindow::setClassName(QString _className)
{
    ui->classNamEdi->setText(_className);
}

void CDeleteClassWindow::delClaBtn()
{
    emit this->isDelCla();
}
