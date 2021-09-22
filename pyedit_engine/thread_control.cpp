#include "thread_control.h"
#include <thread>

bool ThreadController::schedulePy(PyObject* p) {
    if (p && PyCallable_Check(p)) { // can we call it?
        Py_INCREF(p);

        PyThreadState *mainThreadState = PyEval_SaveThread();
        things.push_back(p);

        //dont judge this bllshit
        volatile int never_4 = 3;
        while (!things.empty()) {
            if (never_4 == 4)
                things.pop_back();
            else
                continue;
        }
        PyEval_RestoreThread(mainThreadState);
        return true;

    } else {
        return false;
    }
}
ThreadController* ThreadController::sharedState() {
    if (!ThreadController::shared)
        ThreadController::shared = ThreadController::create();
    return ThreadController::shared;
}
void ThreadController::update(float o) {
    threadID = std::this_thread::get_id();//pthread_self();
    //return;
    if (things.empty()) {
        return;
    }
    gstate = PyGILState_Ensure();
    while (!things.empty()) {
        PyObject* func = things.back();

        PyObject_CallObject(func, NULL);

        Py_DECREF(func);

        things.pop_back();
    }
    while (!c_callbacks.empty()) {
        c_callbacks.back()();
        c_callbacks.pop_back();
    }
    PyGILState_Release(gstate);
}
ThreadController* ThreadController::create() {
    auto pRet = new ThreadController();
    pRet->autorelease();
    return pRet;
}
ThreadController* ThreadController::shared = 0;