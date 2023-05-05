
#include "cpythreadstatelock.h"

CPyThreadStateLock::CPyThreadStateLock()
{
//    state = PyGILState_Ensure();
    _save = nullptr;
    nStatus = 0;
    nStatus = PyGILState_Check();   //检测当前线程是否拥有GIL
    PyGILState_STATE gstate;
    if (!nStatus)
    {
        gstate = PyGILState_Ensure();   //如果没有GIL，则申请获取GIL
        nStatus = 1;
    }
    _save = PyEval_SaveThread();
    PyEval_RestoreThread(_save);
}

CPyThreadStateLock::~CPyThreadStateLock()
{
//    PyGILState_Release(state);
    _save = PyEval_SaveThread();
    PyEval_RestoreThread(_save);
    if (nStatus)
    {
        PyGILState_Release(gstate);    //释放当前线程的GIL
    }
}

