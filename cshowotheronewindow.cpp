#include "cshowotheronewindow.h"
#include "ui_cshowotheronewindow.h"
#include "config.h"

CShowOtherOneWindow::CShowOtherOneWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CShowOtherOneWindow)
{
    ui->setupUi(this);
//    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
//    this->setFixedSize(SHOW_OTHER_ONE_WINDOW_WIDE, SHOW_OTHER_ONE_WINDOW_HIGH);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

CShowOtherOneWindow::~CShowOtherOneWindow()
{
    delete ui;
    emit this->winIsClose();
}

void CShowOtherOneWindow::setlist(QList<std::pair<QString, QString>> strlis1, QList<std::pair<QString, QString>> strlis2)
{
    QString temp1;
    QString temp2;
    qDebug() << "setLost";
    for(int i = 0; i < strlis1.length(); i++)
    {
        if (-1 == strlis2.indexOf(strlis1.at(i)))
            temp1.append(strlis1.at(i).second+"\n");
    }
    for(int i = 0; i < strlis2.length(); i++)
    {
        temp2.append(strlis2.at(i).second+"\n");
    }

    ui->textEdit1->setText(temp1);
    ui->textEdit2->setText(temp2);
}
