#include "cchangeclasswindow.h"
#include "ui_cchangeclasswindow.h"
#include "config.h"

CChangeClassWindow::CChangeClassWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CChangeClassWindow)
{
    ui->setupUi(this);
//    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
//    this->setFixedSize(CHANGE_CLASS_WINDOW_WIDE, CHANGE_CLASS_WINDOW_HIGH);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowModality(Qt::ApplicationModal);
    void (QComboBox::*comCurTextChaSignal)(const QString &text)= &QComboBox::currentTextChanged; //带参函数指针
    connect(ui->comboBox,comCurTextChaSignal,this,&CChangeClassWindow::chaClaComBoxItem);
}

CChangeClassWindow::~CChangeClassWindow()
{
    delete ui;
}

void CChangeClassWindow::chaClaComBoxItem()
{
    emit this->isChaCla();
}

void CChangeClassWindow::setClassList(QStringList _strLis)
{
    ui->comboBox->addItems(_strLis);
}

QString CChangeClassWindow::getNowChange()
{
    return ui->comboBox->currentText();
}

