#include <Python.h>
#include <functional>

#if __APPLE__
#include <cc_defs.hpp>
#else
#include <wind32.h>
#endif

#include <thread>

class ThreadController : public cocos2d::CCNode {
 public:
    bool schedulePy(PyObject* p);
    inline void scheduleC(std::function<void(void)> f) {c_callbacks.push_back(f);};
    static ThreadController* sharedState();
    virtual void update(float o);
    static ThreadController* create();
    static ThreadController* shared;
    inline bool onMain() {return threadID==std::this_thread::get_id();}

    PyGILState_STATE gstate;
    std::vector<PyObject*> things;
    std::vector<std::function<void(void)>> c_callbacks;
    std::thread::id threadID;
};