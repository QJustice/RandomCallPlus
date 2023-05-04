
#include "cmainwindow.h"
#include "./ui_cmainwindow.h"
#include "config.h"
#include "cpythreadstatelock.h"

#include <QDebug>
#define PY_SSIZE_T_CLEAN
#undef slots
#include <Python.h>
#define slots Q_SLOTS
/* 这几行代码是因为python中有个object.h文件中slots的宏定义和Qt中的slots重复定义了，这里我们将python中slots定义为和Qt一样就好了*/
#include <QSqlQuery>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QRandomGenerator>


CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
{

    Py_SetPythonHome((wchar_t *)(L"Python310"));

    if (!Py_IsInitialized())
    {
        //1.初始化Python解释器，这是调用操作的第一步
        Py_Initialize();
        if (!Py_IsInitialized()) {
            qDebug("Initial Python failed!");
            //            emit failed();
        }
        else {

            //执行单句Python语句，用于给出调用模块的路径，否则将无法找到相应的调用模块
            // 初始化线程支持
            PyEval_InitThreads();

            // 启动子线程前执行，为了释放PyEval_InitThreads获得的全局锁，否则子线程可能无法获取到全局锁。
            PyEval_ReleaseThread(PyThreadState_Get());
            qDebug("Initial Python Success!");

        }
    }

    this->ui->setupUi(this);

    this->m_pList = new QStringList;
    this->m_pLukeyList = new QStringList;

    // 计时器
    this->m_pTimer = new QTimer(this);
    this->m_pSysTimer=new QTimer(this);
    this->m_randTime = QTime::currentTime();

    this->m_pSysTimer->start(1000); // 每一千毫秒刷新一次状态栏时间
    this->ui->statusbar->showMessage("Ready", 3000); // 显示临时信息，时间3秒钟.
    this->claNumAndclaNam.first = nullptr;
    this->claNumAndclaNam.second = nullptr;

    // 禁用最大化按钮和禁止调节窗口大小
    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(MAIN_WINDOW_WIDE, MAIN_WINDOW_HIGH);
    this->initDatabase();

    connect(this->m_pSysTimer , &QTimer::timeout, this, &CMainWindow::showSystemTime);
    connect(this->m_pTimer,SIGNAL(timeout()),this,SLOT(handleTimeout()));
    connect(ui->startbtn,&QPushButton::clicked, this, &CMainWindow::startAction);
    connect(ui->otherOnebtn, &QPushButton::clicked, this, &CMainWindow::showOtherOne);
    connect(ui->actionCreateClass, &QAction::triggered, this, &CMainWindow::createClassWindow);
    connect(ui->actionDeleteClass, &QAction::triggered, this, &CMainWindow::deleteClassWindow);
    connect(ui->actionChangeClass, &QAction::triggered, this, &CMainWindow::changeClassWindow);
    connect(ui->actionAddStudent, &QAction::triggered, this, &CMainWindow::addStudenWindow);
    connect(ui->actionDeleteStudent, &QAction::triggered, this, &CMainWindow::deleteStudentWindow);
    connect(ui->actionMaxAnswer, &QAction::triggered, this, &CMainWindow::actionMaxAnsFun);
    connect(ui->actionMinAnswer, &QAction::triggered, this, &CMainWindow::actionMinAnsFun);
    connect(ui->actionMaxScore, &QAction::triggered, this, &CMainWindow::actionMaxSourceFun);
    connect(ui->actionMinScore, &QAction::triggered, this, &CMainWindow::actionMinSourceFun);
    connect(ui->actionCreateDiploma, &QAction::triggered, this, &CMainWindow::createPrizeWindow);
    // void (QComboBox::*comCurTextChaSignal)(const QString &text)= &QComboBox::currentTextChanged; //带参函数指针
    void (QCheckBox::*cheStaSignal)(int arg) = &QCheckBox::stateChanged;
    connect(ui->checkBox, cheStaSignal, this, &CMainWindow::initOneStuOneTimeFuc);
    connect(ui->pushButton, &QPushButton::clicked, this, &CMainWindow::resseting);
}

CMainWindow::~CMainWindow()
{
    this->m_database.close();
    Py_Finalize();
    delete ui;
    delete this->m_pList;
    delete this->m_pLukeyList;
}

void CMainWindow::initDatabase()
{
    if (QSqlDatabase::contains("database_connection"))
    {
        this->m_database = QSqlDatabase::database("database_connection");
    }
    else
    {
        // 建立和SQlite数据库的连接
        this->m_database = QSqlDatabase::addDatabase("QSQLITE", "database_connection");
        // 设置数据库文件的名字
        this->m_database.setDatabaseName("DataBase.db");
    }
}

void CMainWindow::addStudenWindow()
{
    if ("请选择班级" == ui->nowClaLab->text())
    {
        QMessageBox::critical(this, "错误", QString("当前班级非法"));
        return;
    }
    if (!this->m_database.open())
    {
        QMessageBox::critical(this, "错误", QString("数据库表初始化失败，失败原因:%1").arg(this->m_database.lastError().text()));
        return;
    }
    QString queryTemp1 = QString(
                                    "CREATE TABLE IF NOT EXISTS StuClass                            "\
                                    "(                                                              "\
                                    "    StuNum TEXT                                                "\
                                    "           NOT NULL                                            "\
                                    "           REFERENCES Student (StuNum) ON DELETE CASCADE       "\
                                    "                                       ON UPDATE CASCADE       "\
                                    "                                       MATCH FULL,             "\
                                    "    ClassNum TEXT                                              "\
                                    "             REFERENCES Class (ClassNum) ON DELETE NO ACTION   "\
                                    "                                         ON UPDATE NO ACTION   "\
                                    "                                         MATCH FULL            "\
                                    ")STRICT                                                        "
                                );
    QString queryTemp2 = QString(
                                    "CREATE TABLE IF NOT EXISTS Student"\
                                    "(                                 "\
                                    "    StuNum TEXT NOT NULL          "\
                                    "           PRIMARY KEY            "\
                                    "           UNIQUE,                "\
                                    "    StuNam TEXT NOT NULL          "\
                                    ")STRICT                           "
                                );
    QString queryTemp3 = QString(
                                    "CREATE TABLE IF NOT EXISTS StuData                                 "
                                    "(                                                                  "
                                    "    StuNum TEXT PRIMARY KEY                                        "
                                    "           UNIQUE                                                  "
                                    "           NOT NULL                                                "
                                    "           REFERENCES Student (StuNum) ON DELETE NO ACTION         "
                                    "                                       ON UPDATE NO ACTION         "
                                    "                                       MATCH [FULL],               "
                                    "    StuAnsTime    INT  DEFAULT (0),                                "
                                    "    StuAnsRigTime INT  DEFAULT (0),                                "
                                    "    StuSource     TEXT DEFAULT (0)                                 "
                                    ")STRICT                                                            "
                                );
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    sqlQuery.prepare(queryTemp1);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("数据加载失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    sqlQuery.prepare(queryTemp2);
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("数据加载失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    sqlQuery.prepare(queryTemp3);
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("数据加载失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    this->m_addStuWin = new CAddStudentWindow;
    this->m_addStuWin->show();
    connect(this->m_addStuWin, &CAddStudentWindow::isAddStu, this, &CMainWindow::nowAddStu);

}

void CMainWindow::changeClassWindow()
{
    if (!this->m_database.open())
    {
        QMessageBox::critical(this, "错误", QString("数据库表初始化失败，失败原因:%1").arg(this->m_database.lastError().text()));
        return;
    }
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    QString queryTemp = QString("SELECT COUNT(*) FROM sqlite_master where type ='table' and name = 'Class'");
    sqlQuery.prepare(queryTemp);
    // 执行sql语句
    if (!sqlQuery.exec())
        QMessageBox::critical(this, "错误", QString("班级表初始化失败，失败原因:%1").arg(sqlQuery.lastError().text()));
    else if (sqlQuery.next(),0 == sqlQuery.value(0).toInt())
    {
        QMessageBox::information(this, "提示", "不存在任何班级");
        return;
    }
    QString queryTemp1 = QString("SELECT ClassNum, ClassNam FROM Class");
    sqlQuery.prepare(queryTemp1);

    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("班级编号校验失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    else if (sqlQuery.last(), 0 > sqlQuery.at())
    {
        QMessageBox::information(this, "提示", "不存在任何班级");
        return;
    }

    sqlQuery.first();
    QStringList strList;
    strList << "请选择班级";
    while (true)
    {
        strList << (sqlQuery.value(0).toString() + "$" + sqlQuery.value(1).toString());
        if (!sqlQuery.next())
            break;
    }
    this->m_chaClsWin = new CChangeClassWindow;
    this->m_chaClsWin->setClassList(strList);
    this->m_chaClsWin->show();

    connect(this->m_chaClsWin, &CChangeClassWindow::isChaCla, this, &CMainWindow::nowChaCla);
}

void CMainWindow::createClassWindow()
{
    if (!this->m_database.open())
    {
        QMessageBox::critical(this, "错误", QString("数据库表初始化失败，失败原因:%1").arg(this->m_database.lastError().text()));
        return;
    }
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    QString queryTemp = QString(
                                    "CREATE TABLE IF NOT EXISTS Class   "\
                                    "(                                  "\
                                    "    ClassNum TEXT PRIMARY KEY      "\
                                    "             NOT NULL              "\
                                    "             UNIQUE,               "\
                                    "    ClassNam TEXT                  "\
                                    "             NOT NULL              "\
                                    ")STRICT                            "
                                );
    sqlQuery.prepare(queryTemp);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("班级表初始化失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    this->m_creClsWin = new CCreateClassWindow;
    this->m_creClsWin->show();
    connect(this->m_creClsWin, &CCreateClassWindow::isCreCla, this, &CMainWindow::nowCreCla);

}

void CMainWindow::deleteClassWindow()
{
    if (!this->m_database.open())
    {
        QMessageBox::critical(this, "错误", QString("数据库表初始化失败，失败原因:%1").arg(this->m_database.lastError().text()));
        return;
    }
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    // SELECT COUNT(*) FROM sqlite_master where type ='table' and name = 'Class'
    QString queryTemp = QString("SELECT COUNT(*) FROM sqlite_master where type ='table' and name = 'Class'");
    sqlQuery.prepare(queryTemp);
    // 执行sql语句
    if (!sqlQuery.exec())
        QMessageBox::critical(this, "错误", QString("班级表初始化失败，失败原因:%1").arg(sqlQuery.lastError().text()));
    else if (sqlQuery.next(),0 == sqlQuery.value(0).toInt())
    {
        QMessageBox::information(this, "提示", "不存在任何班级");
        return;
    }
    this->m_delClsWin = new CDeleteClassWindow;
    this->m_delClsWin->show();
    connect(this->m_delClsWin, &CDeleteClassWindow::isDelCla, this, &CMainWindow::nowDelCla);
    connect(this->m_delClsWin, &CDeleteClassWindow::claNumChange, this, &CMainWindow::claNumEdiSetClaNam);
}

void CMainWindow::statisticWindow(QString ins)
{
    if ("请选择班级" == ui->nowClaLab->text())
    {
        QMessageBox::critical(this, "错误", QString("当前班级非法"));
        return;
    }
    if (!this->m_database.open())
    {
        QMessageBox::critical(this, "错误", QString("数据库表初始化失败，失败原因:%1").arg(this->m_database.lastError().text()));
        return;
    }
    QString str1 = "StuAnsTime DESC";
    QString str2 = "StuSource DESC";
    QString str3 = "StuAnsTime ASC";
    QString str4 = "StuSource ASC";
    QString strRes = str1;
    if (STU_ANS_TIME_MAX == ins)
        strRes = str1;
    else if (STU_ANS_TIME_MIN == ins)
        strRes = str2;
    else if (STU_SOURCE_MAX == ins)
        strRes = str3;
    else if (STU_SOURCE_MIN == ins)
        strRes = str4;
    else
        ;
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    // SELECT COUNT(*) FROM sqlite_master where type ='table' and name = 'Class'
    QString queryTemp = QString(
                                    "SELECT Student.StuNum, StuNam, StuAnsTime, StuAnsRigTime, StuSource    "
                                    "FROM Student, StuClass, StuData                                        "
                                    "WHERE Student.StuNum = StuClass.StuNum                                 "
                                    "AND Student.StuNum = StuData.StuNum                                    "
                                    "AND StuClass.ClassNum = '%1'                                           "
                                    "ORDER BY %2                                                            "
                                ).arg(this->claNumAndclaNam.first, strRes);
    sqlQuery.prepare(queryTemp);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("数据查询失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    QStringList table_h_headers;
    table_h_headers << "学号" << "姓名" << "回答总数" << "回答正确数" << "回答正确率"<< "总得分";
    this->m_statisticWin = new CStatisticsWindow;
    this->m_statisticWin->setTabelColRowCount((sqlQuery.last(), sqlQuery.at()) + 1, table_h_headers.size());
    this->m_statisticWin->setTableHead(table_h_headers);

    int col = 0;
    int row = 0;
    int colMax = MAX_COLUMN_COUNT;
    sqlQuery.first();
    do
    {
        this->m_statisticWin->setTabelItem(row, col++, sqlQuery.value(0).toString());
        this->m_statisticWin->setTabelItem(row, col++, sqlQuery.value(1).toString());
        this->m_statisticWin->setTabelItem(row, col++, sqlQuery.value(2).toString());
        this->m_statisticWin->setTabelItem(row, col++, sqlQuery.value(3).toString());
        if (0 != sqlQuery.value(2).toDouble())
            this->m_statisticWin->setTabelItem(row, col++,QString::number((sqlQuery.value(3).toDouble() / sqlQuery.value(2).toDouble())*100) + "%");
        else
            this->m_statisticWin->setTabelItem(row, col++, "0");
        this->m_statisticWin->setTabelItem(row, col++, sqlQuery.value(4).toString());
        if (colMax - 1 == col)
        {
            col = 0;
            row++;
        }

    } while (sqlQuery.next());

    this->m_statisticWin->show();
}

void CMainWindow::createPrizeWindow()
{

    this->m_crePriWin = new CCreatePrizeWindow;
    this->m_crePriWin->show();

    connect(this->m_crePriWin, &CCreatePrizeWindow::isCreatePrize, this, &CMainWindow::nowCrePri);
}

void CMainWindow::handleTimeout()
{
    // 生成随机数范围为0~未出场人数最大下标减去1
    int randNum = QRandomGenerator::global()->bounded(0,this->m_pList->size());
    ui->label->setText(QString(this->m_pList->at(randNum)));
}

void CMainWindow::readList()
{
    if (!this->m_pList->empty())
        return;
    this->m_errCode = 0;
    if (!this->m_database.open())
    {
        QMessageBox::critical(this, "错误", QString("数据库表初始化失败，失败原因:%1").arg(this->m_database.lastError().text()));
        this->m_errCode = 1;
        return;
    }
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    QString queryTemp1 = QString("SELECT COUNT(*) FROM sqlite_master where type ='table' and name = 'StuClass'");
    sqlQuery.prepare(queryTemp1);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("数据库加载失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        this->m_errCode = 1;
        return;
    }
    else if (sqlQuery.next(),0 == sqlQuery.value(0).toInt())
    {
        QMessageBox::information(this, "提示", "不存在可用数据");
        this->m_errCode = 2;
        return;
    }
    QString queryTemp2 = QString("SELECT COUNT(*) FROM StuClass WHERE ClassNum='%1'").arg(this->claNumAndclaNam.first);
    sqlQuery.prepare(queryTemp2);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("数据库加载失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        this->m_errCode = 1;
        return;
    }
    else if (sqlQuery.next(),0 == sqlQuery.value(0).toInt())
    {
        QMessageBox::information(this, "提示", "当前班级没有学生");
        this->m_errCode = 2;
        return;
    }
    QString queryTemp3 = QString(
                                    "SELECT StuNam                                                      "
                                    "FROM StuClass, Student                                             "
                                    "WHERE StuClass.StuNum=Student.StuNum AND StuClass.ClassNum='%1'    "
                                ).arg(this->claNumAndclaNam.first);
    sqlQuery.prepare(queryTemp3);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("数据库加载失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        this->m_errCode = 1;
    }
    while(sqlQuery.next())
    {
        this->m_pList->append(sqlQuery.value(0).toString());
        qDebug() << sqlQuery.value(0).toString();
    }
    QStringList distin;
    this->m_pList->removeAll(QString("")); // Returns the number of entries removed
    for(int i = 0; i < this->m_pList->length(); i++)
    {
        // 找出是否有相同的
        if (!distin.contains((*(this->m_pList))[i]))
            distin.append((*(this->m_pList))[i]);
    }
    *(this->m_pList) = distin;
    this->m_allNum = this->m_pList->length();
    this->m_pLukeyList->clear();
    qDebug() << *(this->m_pList) << "read";

}

void CMainWindow::showOtherOne()
{
    if (nullptr == this->claNumAndclaNam.first)
    {
        QMessageBox::critical(this, "错误", QString("请先选择班级"));
        return;
    }
    if (this->m_pList->empty() && this->m_pLukeyList->empty())
        this->readList();
    if (0 != this->m_errCode)
        return;
    if (!this->m_showOthOneWin)
        this->m_showOthOneWin = new CShowOtherOneWindow;
    else
        this->m_showOthOneWin->activateWindow();
    this->m_showOthOneWin->setlist(*(this->m_pList), *(this->m_pLukeyList));
    this->m_showOthOneWin->show();
}

void CMainWindow::showSystemTime()
{
    this->m_dateTime = this->m_dateTime.currentDateTime();
    this->ui->statusbar->showMessage(m_dateTime.toString("yyyy-MM-dd hh:mm:ss ddd"));
}

void CMainWindow::startAction()
{
    if (nullptr == this->claNumAndclaNam.first)
    {
        QMessageBox::critical(this, "错误", QString("请先选择班级"));
        return;
    }
    this->readList();
    if (this->m_errCode==0)
    {
        if (this->m_pTimer->isActive())
        {
            ui->startbtn->setText("开始");
            this->m_pTimer->stop();
            if (ui->checkBox->isChecked())
            {
                this->m_pList->removeOne(ui->label->text());
                this->m_pLukeyList->append(ui->label->text());
            }
            else if (-1 == this->m_pLukeyList->indexOf(ui->label->text()))
            {
                this->m_pLukeyList->append(ui->label->text());
            }
            else
                return;
            ui->labelNum->setText(QString("%1/%2(已出场/全部)").arg(this->m_pLukeyList->length()).arg(this->m_allNum));
            qDebug() << this->m_pLukeyList;
        }
        else
        {
            ui->startbtn->setText("停止");
            this->m_pTimer->start(50);
        }
    }
    else if (this->m_errCode==1)
        ;
        // QMessageBox::critical(this,"出错了","疑似数据库异常");
    else if (this->m_errCode==2)
        ;
        // QMessageBox::critical(this,"出错了","当前班级没有学生无法进行点名");

}

void CMainWindow::nowAddStu()
{
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    QString studentNumber = "studentNumber";
    QString studentName = "studentName";

    studentNumber = this->m_addStuWin->getStudentNumber();
    studentName = this->m_addStuWin->getStudentsName();
    if ("" == studentNumber || "" == studentName)
    {
        QMessageBox::information(this, "提示", "学生编号或学生名称不可为空");
        return;
    }
    QString queryTemp1 = QString(
                                    "SELECT COUNT(*)              "\
                                    "FROM Student WHERE StuNum = '%1'   "
                                ).arg(studentNumber);

    sqlQuery.prepare(queryTemp1);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("数据加载失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    else if (sqlQuery.next(),0 != sqlQuery.value(0).toInt())
    {
        QMessageBox::information(this, "提示", "已存在此学生, 不可重复添加");
        return;
    }
    QString queryTemp2 = QString("INSERT INTO Student(StuNum, StuNam) "
                                "VALUES ('%1', '%2')").arg(studentNumber, studentName);
    sqlQuery.prepare(queryTemp2);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("学生添加失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    QString queryTemp3 = QString("INSERT INTO StuClass(StuNum, ClassNum) "
                                 "VALUES ('%1', '%2')").arg(studentNumber, this->claNumAndclaNam.first);
    sqlQuery.prepare(queryTemp3);
    if (!sqlQuery.exec())
    {
        this->m_addStuWin->close();
        QMessageBox::critical(this, "错误", QString("学生班级信息加载失败，失败原因:%1\n即将自动回滚上一步操作").arg(sqlQuery.lastError().text()));
        // 添加数据库回滚操作
        QString queryTemp4 = QString("DELETE  FROM  Student WHERE  StuNum = '%1' AND StuNam = '%2'").arg(studentNumber, studentName);
        sqlQuery.prepare(queryTemp4);
        // 执行sql语句
        if (!sqlQuery.exec())
        {
            QMessageBox::critical(this, "错误", QString("数据回滚失败,请检查学生数据，失败原因:%1").arg(sqlQuery.lastError().text()));
        }
        else
        {
            QMessageBox::information(this, "提示", "数据回滚成功");
        }
        return;
    }
    QString queryTemp5 = QString("INSERT INTO StuData(StuNum) "
                                 "VALUES ('%1')").arg(studentNumber);
    sqlQuery.prepare(queryTemp5);
    if (!sqlQuery.exec())
    {
        this->m_addStuWin->close();
        QMessageBox::critical(this, "错误", QString("学生班级信息加载失败，失败原因:%1\n即将自动回滚上一步操作").arg(sqlQuery.lastError().text()));
        if (!sqlQuery.exec("PRAGMA foreign_keys=ON"))
        {
            QMessageBox::critical(this, "错误", QString("数据库异常"));
        }
        // 添加数据库回滚操作
        QString queryTemp6 = QString("DELETE  FROM  Student WHERE  StuNum = '%1' AND StuNam = '%2'").arg(studentNumber, studentName);
        sqlQuery.prepare(queryTemp6);
        // 执行sql语句
        if (!sqlQuery.exec())
        {
            QMessageBox::critical(this, "错误", QString("数据回滚失败,请检查学生数据，失败原因:%1").arg(sqlQuery.lastError().text()));
        }
        else
        {
            QMessageBox::information(this, "提示", "数据回滚成功");
        }
        return;
    }
    this->m_addStuWin->close();
    QMessageBox::information(this, "提示", "学生添加成功");
}

void CMainWindow::nowCreCla()
{
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    QString classNumber = "classNumber";
    QString className = "className";

    classNumber = this->m_creClsWin->getClassNumber();
    className = this->m_creClsWin->getClassName();

    if ("" == classNumber || "" == className)
    {
        QMessageBox::information(this, "提示", "班级编号或班级名称不可为空");
        return;
    }
    QString queryTemp1 = QString("SELECT COUNT(*) FROM Class WHERE ClassNum = '%1'").arg(classNumber);

    sqlQuery.prepare(queryTemp1);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("班级编号校验失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    else if (sqlQuery.next(),0 != sqlQuery.value(0).toInt())
    {
        QMessageBox::information(this, "提示", "班级编号已存在");
        return;
    }
    QString queryTemp2 = QString("INSERT INTO Class(ClassNum, ClassNam) "
                                "VALUES ('%1', '%2')").arg(classNumber, className);
    sqlQuery.prepare(queryTemp2);
    // 执行sql语句
    if (!sqlQuery.exec())
        QMessageBox::critical(this, "错误", QString("班级创建失败，失败原因:%1").arg(sqlQuery.lastError().text()));
    else
    {
        QMessageBox::information(this, "提示", "班级创建成功");
        this->m_creClsWin->close();
    }
}

void CMainWindow::nowDelCla()
{
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    QString classNumber = "classNumber";
    QString className = "className";
    classNumber = this->m_delClsWin->getClassNumber();
    className = this->m_delClsWin->getClassName();
    QString queryTemp1 = QString("SELECT ClassNum, ClassNam FROM Class WHERE ClassNum = '%1'").arg(classNumber);
    sqlQuery.prepare(queryTemp1);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("班级编号校验失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    else if (sqlQuery.last(), 0 > sqlQuery.at())
    {
        QMessageBox::information(this, "提示", "班级编号不存在");
        return;
    }
    else
    {
        this->m_delClsWin->setClassName(sqlQuery.value(1).toString());
    }
    QString queryTemp2 = QString("DELETE  FROM  Class WHERE  ClassNum = '%1' AND ClassNam = '%2'").arg(classNumber, className);
    sqlQuery.prepare(queryTemp2);
    // 执行sql语句
    if (!sqlQuery.exec())
        QMessageBox::critical(this, "错误", QString("班级删除失败，失败原因:%1").arg(sqlQuery.lastError().text()));
    else
    {
        this->m_delClsWin->close();
        QMessageBox::information(this, "提示", "班级删除成功");
    }
}

void CMainWindow::nowChaCla()
{
    QString sourceStr = this->m_chaClsWin->getNowChange();
    ui->nowClaLab->setText(sourceStr);
    QString firstStr, secondStr;
    int begin = 0;
    int end = sourceStr.size() - 1;
    while (begin < end)
    {
        if (sourceStr.at(begin) != '$' )
        {
            firstStr.append(sourceStr.at(begin));
            begin++;
        }
        if (sourceStr.at(end) != '$' )
        {
            secondStr.push_front(sourceStr.at(end));
            end--;
        }
    }
    this->claNumAndclaNam.first = firstStr;
    this->claNumAndclaNam.second = secondStr;
    this->readList();
    this->resseting();
}

void CMainWindow::nowDelStu()
{
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    QString studentNumber = "studentNumber";
    QString studentName = "studentName";

    studentNumber = this->m_delStuWin->getStudentNumber();
    studentName = this->m_delStuWin->getStudentsName();
    //    this->m_addStuWin->setClassName(ui->nowClaLab->text());
    if ("" == studentNumber || "" == studentName)
    {
        QMessageBox::information(this, "提示", "学生编号或学生名称不可为空");
        return;
    }
    QString queryTemp1 = QString("SELECT StuNum, StuNam FROM Student WHERE StuNum = '%1'").arg(studentNumber);
    sqlQuery.prepare(queryTemp1);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("数据加载失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    else if (sqlQuery.last(), 0 > sqlQuery.at())
    {
        QMessageBox::information(this, "提示", "不存在此学生");
        return;
    }
    else
    {
        this->m_delStuWin->setStudentName(sqlQuery.value(1).toString());
    }
    QString queryTemp2 = QString("SELECT StuNum, ClassNum FROM StuClass WHERE StuNum = '%1'").arg(studentNumber);
    sqlQuery.prepare(queryTemp2);
    // 执行sql语句
    if (!sqlQuery.exec())
        // this->claNumAndclaNam.first
        QMessageBox::critical(this, "错误", QString("学生删除失败，失败原因:%1").arg(sqlQuery.lastError().text()));
    else if (sqlQuery.last(), 0 > sqlQuery.at())
    {
        QMessageBox::information(this, "提示", "不存在任何学生");
        return;
    }
    while (true)
    {
        if (this->claNumAndclaNam.first == sqlQuery.value(1))
            break;
        if (!sqlQuery.next())
        {
            QMessageBox::critical(this, "错误", QString("需删除学生不隶属于当前班级，系统拒绝删除"));
            return;
        }
    }

    if (!sqlQuery.exec("PRAGMA foreign_keys=ON"))
    {
        QMessageBox::critical(this, "错误", QString("数据库异常"));
    }
    QString queryTemp3 = QString("DELETE FROM Student WHERE StuNum = '%1' AND StuNam = '%2'").arg(studentNumber, studentName);
    sqlQuery.prepare(queryTemp3);
    // 执行sql语句
    if (!sqlQuery.exec())
        QMessageBox::critical(this, "错误", QString("学生删除失败，失败原因:%1").arg(sqlQuery.lastError().text()));
    else
    {
        this->m_delStuWin->close();
        QMessageBox::information(this, "提示", "学生删除成功");
    }
}

void CMainWindow::nowCrePri()
{
    QStringList stuName = this->m_crePriWin->getStuNameList();
    qDebug() << this->m_crePriWin->getDipTemPath();
    qDebug() << this->m_crePriWin->getStuNameList();
    qDebug() << this->m_crePriWin->getFlaKey();
    qDebug() << this->m_crePriWin->getBuiFolPath();


    PyObject* pModule = NULL;
    PyObject* pFunc = NULL;
    PyObject* pName = NULL;
    //2、初始化python系统文件路径，保证可以访问到 .py文件
    //    PyRun_SimpleString("import sys");
    //    PyRun_SimpleString("sys.path.append('./')");

    //3、调用python文件名。当前的测试python文件名是 handleword.py
    // 在使用这个函数的时候，只需要写文件的名称就可以了。不用写后缀。
    class CPyThreadStateLock PyThreadLock; // 获取全局锁
    qDebug() << "1";
    pModule = PyImport_ImportModule("handleword");
    //4、调用函数
    qDebug() << "2";
    pFunc = PyObject_GetAttrString(pModule, "write_word");
    //5、给python传参数
    // 函数调用的参数传递均是以元组的形式打包的,2表示参数个数
    // 如果AdditionFc中只有一个参数时，写1就可以了
    qDebug() << "3";
    PyObject* pArgs = PyTuple_New(4);

    // 7、接收python计算好的返回值
    int nResult;
    qDebug() << "4";
    for (int i = 0; i < stuName.size(); i++)
    {
//        qDebug() << "i" << i;
//        qDebug() << "stusize" << stuName.size();
//        qDebug() << stuName.at(i).toStdString().c_str();

        // 0：第一个参数，传入 int 类型的值 2
        PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", this->m_crePriWin->getDipTemPath().toStdString().c_str()));
        // 1：第二个参数，传入 int 类型的值 4
        PyTuple_SetItem(pArgs, 1, Py_BuildValue("s", stuName.at(i).toStdString().c_str()));
        // 2：第三个参数，传入 int 类型的值 2
        PyTuple_SetItem(pArgs, 2, Py_BuildValue("s", this->m_crePriWin->getFlaKey().toStdString().c_str()));
        // 3：第四个参数，传入 int 类型的值 4
        PyTuple_SetItem(pArgs, 3, Py_BuildValue("s", this->m_crePriWin->getBuiFolPath().toStdString().c_str()));
//        qDebug() << "is here";
//        qDebug() << this->m_crePriWin->getFlaKey().toStdString().c_str();
        // 6、使用C++的python接口调用该函数
        PyObject* pReturn = PyObject_CallObject(pFunc, pArgs);


        // i表示转换成int型变量。
        // 在这里，最需要注意的是：PyArg_Parse的最后一个参数，必须加上“&”符号
        PyArg_Parse(pReturn, "i", &nResult);
        qDebug() << "return result is " << nResult;
    }

    qDebug() << "ok";
}

void CMainWindow::claNumEdiSetClaNam()
{
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    QString classNumber = "classNumber";
    QString className = "className";
    classNumber = this->m_delClsWin->getClassNumber();
    className = this->m_delClsWin->getClassName();
    QString queryTemp1 = QString("SELECT ClassNum, ClassNam FROM Class WHERE ClassNum = '%1'").arg(classNumber);
    sqlQuery.prepare(queryTemp1);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("班级编号校验失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    else if (sqlQuery.last(), 0 > sqlQuery.at())
    {
        this->m_delClsWin->setClassName("");
        return;
    }
    else
    {
        this->m_delClsWin->setClassName(sqlQuery.value(1).toString());
    }
}

void CMainWindow::deleteStudentWindow()
{
    if (!this->m_database.open())
    {
        QMessageBox::critical(this, "错误", QString("数据库表初始化失败，失败原因:%1").arg(this->m_database.lastError().text()));
        return;
    }
    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    // SELECT COUNT(*) FROM sqlite_master where type ='table' and name = 'Class'
    QString queryTemp = QString("SELECT COUNT(*) FROM sqlite_master where type ='table' and name = 'Student'");
    sqlQuery.prepare(queryTemp);
    // 执行sql语句
    if (!sqlQuery.exec())
        QMessageBox::critical(this, "错误", QString("数据加载失败，失败原因:%1").arg(sqlQuery.lastError().text()));
    else if (sqlQuery.next(),0 == sqlQuery.value(0).toInt())
    {
        QMessageBox::information(this, "提示", "不存在任何学生");
        return;
    }
    QString queryTemp1 = QString("SELECT StuNum, StuNam FROM Student");
    sqlQuery.prepare(queryTemp1);

    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("数据加载失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    else if (sqlQuery.last(), 0 > sqlQuery.at())
    {
        QMessageBox::information(this, "提示", "不存在任何学生");
        return;
    }
    this->m_delStuWin = new CDeleteStudentWindow;
    this->m_delStuWin->show();

    connect(this->m_delStuWin, &CDeleteStudentWindow::isDelStu, this, &CMainWindow::nowDelStu);
    connect(this->m_delStuWin, &CDeleteStudentWindow::stuNumChange, this, &CMainWindow::stuNumEdiSetStuNam);
}

void CMainWindow::stuNumEdiSetStuNam()
{

    QSqlQuery sqlQuery(this->m_database); // 用于执行sql语句的对象
    QString studentNumber = "studentNumber";
    QString studentName = "studentName";

    studentNumber = this->m_delStuWin->getStudentNumber();
    studentName = this->m_delStuWin->getStudentsName();
    //    this->m_addStuWin->setClassName(ui->nowClaLab->text());
    QString queryTemp1 = QString("SELECT StuNum, StuNam FROM Student WHERE StuNum = '%1'").arg(studentNumber);
    sqlQuery.prepare(queryTemp1);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        QMessageBox::critical(this, "错误", QString("数据加载失败，失败原因:%1").arg(sqlQuery.lastError().text()));
        return;
    }
    else if (sqlQuery.last(), 0 > sqlQuery.at())
    {
        this->m_delStuWin->setStudentName("");
        return;
    }
    else
    {
        this->m_delStuWin->setStudentName(sqlQuery.value(1).toString());
    }
}

void CMainWindow::initOneStuOneTimeFuc(int arg)
{
    if (2 == arg)
        for (int i = 0; i < this->m_pLukeyList->length(); i++)
            this->m_pList->removeOne(m_pLukeyList->at(i));
    if (0 == arg)
        this->m_pList->clear();
    this->readList();
}

void CMainWindow::resseting()
{
    ui->label->setText("开始");
    ui->labelNum->setText("0/0(已出场/全部)");
    this->m_pList->clear();
    this->m_pLukeyList->clear();
}

void CMainWindow::actionMaxAnsFun()
{
    this->statisticWindow(STU_ANS_TIME_MAX);
}

void CMainWindow::actionMinAnsFun()
{
    this->statisticWindow(STU_ANS_TIME_MIN);
}

void CMainWindow::actionMaxSourceFun()
{
    this->statisticWindow(STU_SOURCE_MAX);
}

void CMainWindow::actionMinSourceFun()
{
    this->statisticWindow(STU_SOURCE_MIN);
}
