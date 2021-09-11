#include <Python.h>
#include <cc_defs.hpp>
#include <pthread.h>
#include <functional>

class ThreadController : public cocos2d::CCNode {
 public:
    bool schedulePy(PyObject* p);
    inline void scheduleC(std::function<void(void)> f) {c_callbacks.push_back(f);};
    static ThreadController* sharedState();
    virtual void update(float o);
    static ThreadController* create();
    static ThreadController* shared;
    inline bool onMain() {return threadID==pthread_self();}

    PyGILState_STATE gstate;
    std::vector<PyObject*> things;
    std::vector<std::function<void(void)>> c_callbacks;
    pthread_t threadID;
};