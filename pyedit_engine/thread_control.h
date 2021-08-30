#include <Python.h>
#include <cc_defs.hpp>
#include <pthread.h>

class ThreadController : public cocos2d::CCNode {
 public:
    bool schedulePy(PyObject* p);
    static ThreadController* sharedState();
    virtual void update(float o);
    static ThreadController* create();
    static ThreadController* shared;
    inline bool onMain() {return threadID==pthread_self();}

    PyGILState_STATE gstate;
    std::vector<PyObject*> things;
    pthread_t threadID;
};