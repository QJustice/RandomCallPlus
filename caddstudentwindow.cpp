#include "caddstudentwindow.h"
#include "ui_caddstudentwindow.h"
#include "config.h"

CAddStudentWindow::CAddStudentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CAddStudentWindow)
{
    ui->setupUi(this);
//    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
//    this->setFixedSize(ADD_STUDENT_WINDOW_WIDE, ADD_STUDENT_WINDOW_HIGH);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowModality(Qt::ApplicationModal);
    connect(ui->addStuBtn, &QPushButton::clicked, this, &CAddStudentWindow::isAddStu);
    connect(ui->actionAddStu, &QAction::triggered, this, &CAddStudentWindow::isAddStu);
    connect(ui->actionExit, &QAction::triggered, this, &CAddStudentWindow::close);
}

CAddStudentWindow::~CAddStudentWindow()
{
    delete ui;
}

QString CAddStudentWindow::getStudentNumber()
{
    return ui->stuNumEdi->text();
}

QString CAddStudentWindow::getStudentsName()
{
    return ui->stuNamEdi->text();
}
