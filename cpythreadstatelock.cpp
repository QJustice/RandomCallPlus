
#include "cpythreadstatelock.h"

CPyThreadStateLock::CPyThreadStateLock()
{
    state = PyGILState_Ensure();
}

CPyThreadStateLock::~CPyThreadStateLock()
{
    PyGILState_Release(state);
}

