#include "ccreateclasswindow.h"
#include "ui_ccreateclasswindow.h"
#include "config.h"
#include <QDebug>

CCreateClassWindow::CCreateClassWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CCreateClassWindow)
{
    ui->setupUi(this);
    // 禁用最大化按钮和禁止调节窗口大小
    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(CREATE_CLASS_WINDOW_WIDE, CREATE_CLASS_WINDOW_HIGH);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowModality(Qt::ApplicationModal);
    connect(ui->creClaBtn, &QPushButton::clicked, this, &CCreateClassWindow::creClaBtn);
    connect(ui->actionCreate, &QAction::triggered, this, &CCreateClassWindow::creClaBtn);
    connect(ui->actionExit, &QAction::triggered, this, &CCreateClassWindow::close);
}

CCreateClassWindow::~CCreateClassWindow()
{
    delete ui;
    qDebug() << "is Delete";
}

QString CCreateClassWindow::getClassNumber()
{
    return ui->classNumEdi->text();
}

QString CCreateClassWindow::getClassName()
{
    return ui->classNamEdi->text();
}


void CCreateClassWindow::creClaBtn()
{
    emit this->isCreCla();
}

