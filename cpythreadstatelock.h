
#ifndef CPYTHREADSTATELOCK_H
#define CPYTHREADSTATELOCK_H

//将全局解释器锁和线程的相关操作用类封装
#define PY_SSIZE_T_CLEAN
#undef slots
#include <Python.h>
#define slots Q_SLOTS


class CPyThreadStateLock
{
public:
    CPyThreadStateLock();
    ~CPyThreadStateLock();
private:
    PyGILState_STATE gstate;
    PyThreadState *_save;
    int nStatus;
};

#endif // CPYTHREADSTATELOCK_H


