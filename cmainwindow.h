
#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

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
    QStringList *m_pList = nullptr;                                         // 名单数据
    QStringList *m_pLukeyList = nullptr;                                    // 已经出场名单
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



    int m_errCode = 0;                                                      //状态码：0读取名单文件成功 1没有找到名单文件，2名单文件读取失败
    int m_allNum = 0;                                                       // 所有学生数量

private slots:
    void addStudenWindow();                                                 // 添加学生窗口
    void changeClassWindow();                                               // 更换当前班级窗口
    void createClassWindow();                                               // 建立创建班级窗口
    void deleteClassWindow();                                               // 删除班级窗口
    void statisticWindow();                                                 // 统计窗口
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
    void claNumEdiSetClaNam();                                              // 自动更新班级名
    void deleteStudentWindow();                                             // 删除学生
    void stuNumEdiSetStuNam();                                              // 自动更新学生名
    void initOneStuOneTimeFuc(int arg);                                     // 初始化雨露均沾功能
    void resseting();                                                       // 重置程序

};

#endif // CMAINWINDOW_H
