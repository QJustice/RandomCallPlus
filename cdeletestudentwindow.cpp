#include "cdeletestudentwindow.h"
#include "ui_cdeletestudentwindow.h"
#include "config.h"
#include <QDebug>

CDeleteStudentWindow::CDeleteStudentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CDeleteStudentWindow)
{
    ui->setupUi(this);
//    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
//    this->setFixedSize(DELETE_STUDENT_WINDOW_WIDE, DELETE_STUDENT_WINDOW_HIGH);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowModality(Qt::ApplicationModal);
    connect(ui->delStuBtn, &QPushButton::clicked, this, &CDeleteStudentWindow::isDelStu);
    connect(ui->stuNumEdi, &QLineEdit::textEdited, this, &CDeleteStudentWindow::stuNumChange);
}

CDeleteStudentWindow::~CDeleteStudentWindow()
{
    delete ui;
}

QString CDeleteStudentWindow::getStudentNumber()
{
    return ui->stuNumEdi->text();
}

QString CDeleteStudentWindow::getStudentsName()
{
    return ui->stuNamEdi->text();
}

void CDeleteStudentWindow::setStudentName(QString _studentName)
{
    ui->stuNamEdi->setText(_studentName);
}
