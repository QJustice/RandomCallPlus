
#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#define PY_SSIZE_T_CLEAN
#undef slots
#include <Python.h>
#define slots Q_SLOTS
/* 这几行代码是因为python中有个object.h文件中slots的宏定义和Qt中的slots重复定义了，这里我们将python中slots定义为和Qt一样就好了*/

#include <QMainWindow>
#include <QTime>
#include <QSqlDatabase>

#include <utility>

#include "ccreateclasswindow.h"
#include "cdeleteclasswindow.h"
#include "cchangeclasswindow.h"
#include "caddstudentwindow.h"
#include "cdeletestudentwindow.h"
#include "cshowotheronewindow.h"
#include "cstatisticswindow.h"
#include "ccreateprizewindow.h"
#include "cstudentdatalist.h"


QT_BEGIN_NAMESPACE
namespace Ui { class CMainWindow; }
QT_END_NAMESPACE

class CMainWindow : public QMainWindow

{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();
    void initDatabase();

private:
    Ui::CMainWindow *ui;
    QTimer *m_pSysTimer = nullptr;
    QTimer *m_pTimer = nullptr;                                             // 计时器
    QTime m_randTime;                                                       // 生成随机数
    QList<std::pair<QString, QString>> *m_pList = nullptr;                  // 名单数据
    QList<std::pair<QString, QString>> *m_pLukeyList = nullptr;             // 已经出场名单
    QDateTime m_dateTime;                                                   // 系统时间
    QSqlDatabase m_database;                                                // 数据库对象
    CCreateClassWindow *m_creClsWin = nullptr;                              // 创建班级界面
    CDeleteClassWindow *m_delClsWin = nullptr;                              // 删除班级界面
    std::pair<QString, QString> claNumAndclaNam;                            // 班级编号和班级名称
    CChangeClassWindow *m_chaClsWin = nullptr;                              // 更换班级界面
    CAddStudentWindow *m_addStuWin = nullptr;                               // 添加学生窗口
    CDeleteStudentWindow  *m_delStuWin = nullptr;                           // 删除学生窗口
    CShowOtherOneWindow *m_showOthOneWin = nullptr;                         // 学生出场情况查看窗口
    CStatisticsWindow *m_statisticWin = nullptr;                            // 统计数据窗口
    CCreatePrizeWindow *m_crePriWin = nullptr;                              // 生成奖状窗口
    std::pair<QString, QString> m_lukeyOne;                                 // 当前被选中在的学生
    CStudentDataList *m_stuDataListWin = nullptr;                           // 导入学生窗口

    int m_errCode = 0;                                                      // 状态码：0读取名单文件成功 1没有找到名单文件，2名单文件读取失败
    int m_allNum = 0;                                                       // 所有学生数量

private slots:
    void addStudenWindow();                                                 // 添加学生窗口
    void changeClassWindow();                                               // 更换当前班级窗口
    void createClassWindow();                                               // 建立创建班级窗口
    void deleteClassWindow();                                               // 删除班级窗口
    void statisticWindow(QString ins);                                      // 统计窗口
    void createPrizeWindow();                                               // 生成奖品窗口
    void handleTimeout();
    void readList();
    void showOtherOne();
    void showSystemTime();                                                  // 显示系统时间
    void startAction();
    void nowAddStu();                                                       // 添加学生
    void nowCreCla();                                                       // 创建班级
    void nowDelCla();                                                       // 删除班级
    void nowChaCla();                                                       // 更换班级
    void nowDelStu();                                                       // 删除学生
    void nowCrePri();                                                       // 生成奖状
    void nowImportStuData();                                                // 导入学生数据
    void claNumEdiSetClaNam();                                              // 自动更新班级名
    void deleteStudentWindow();                                             // 删除学生
    void stuNumEdiSetStuNam();                                              // 自动更新学生名
    void initOneStuOneTimeFuc(int arg);                                     // 初始化雨露均沾功能
    void resseting();                                                       // 重置程序
    void actionMaxAnsFun();                                                 // 最多回答
    void actionMinAnsFun();                                                 // 最少回答
    void actionMaxSourceFun();                                              // 最多得分
    void actionMinSourceFun();                                              // 最少得分
    void addStudentSource();                                                // 增加分数
    void addStudentlist();                                                  // 导入学生

};

#endif // CMAINWINDOW_H
