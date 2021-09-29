#include <engine.h>
#include <Python.h>
#include <thread_control.h>
#include <vector>
#include <helper_hooks.h>
#include <mutex>
using namespace cocos2d;
#include "cy/main.h"

#if __APPLE__
#include <Cacao.hpp>
#include <CoreGraphics/CoreGraphics.h>
#else
#include <wind32.h>
#endif

static PyObject* PyGD_alert(PyObject *self, PyObject *args) {
    char* title;
    char* des;
    char* button;
    if(!PyArg_ParseTuple(args, "sss:alert", &title, &des, &button)) // get the arg
        return NULL;

    ThreadController::sharedState()->scheduleC([=](){
        auto fl = FLAlertLayer::create(NULL, title, button, NULL, std::string(des));
        fl->show();
    });
    Py_RETURN_NONE;
}

static PyMethodDef PyGDMethods[] = {
    {"alert", PyGD_alert, METH_VARARGS, "Simple FLAlertLayer creator"},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef PyGDModule = {
    PyModuleDef_HEAD_INIT, "pygd", NULL, -1, PyGDMethods,
    NULL, NULL, NULL, NULL
};

static PyObject*
PyInit_PyGD(void)
{
    return PyModule_Create(&PyGDModule);
}

namespace engine {
    #if __APPLE__
    static CFMachPortRef stopRef;
    #endif

    static bool haltOn = false;
    static uint32_t timeSinceOn = 0;
    std::vector<std::pair<std::string, std::string>> engineQueue;
    static std::mutex threadSafety;
    static volatile bool pyRunning;

    void engineThread() {
        PyImport_AppendInittab("pygd", &PyInit_PyGD);
        PyImport_AppendInittab("EditorUI", &PyInit_main);

        Py_Initialize();
        PyEval_InitThreads(); 

        pyThreadID = std::this_thread::get_id();
        PyRun_SimpleString("global_storage = {}");

        while (true) {
            if (engineQueue.size() > 0) {

                threadSafety.lock();
                pyRunning = true;
                threadSafety.unlock();

                auto file = engineQueue.back().first.c_str();
                std::cout<<"look its "<<engineQueue.back().second<<"\n";
                
                FILE* fp = fopen(file, "r");
                if (!fp) {
                    printf("kinda bullshit if you ask me\n");
                    engineQueue.pop_back();
                    PyErr_Print();
                    threadSafety.lock();
                    pyRunning = false;
                    threadSafety.unlock();
                    continue;
                }
                PyRun_SimpleString("(lambda x:[globals().clear(),globals().update({'global_storage': x})])(global_storage)\nfrom EditorUI import *\n");

                PyObject* m = PyImport_AddModule("__main__");
                if (!m) {
                    printf("are you fucking serious rn\n");
                    engineQueue.pop_back();
                    PyErr_Print();
                    PyRun_SimpleString("print(\"this is stpid\")");
                    threadSafety.lock();
                    pyRunning = false;
                    threadSafety.unlock();
                    continue;
                }

                //PyObject_SetAttrString(m,"Keybind", PyUnicode_FromString(keybind));
                PyModule_AddStringConstant(m, "Keybind", engineQueue.back().second.c_str());
                engineQueue.pop_back();

                PyRun_SimpleFile(fp, file);
                printf("finished running file\n");
                fclose(fp);

                threadSafety.lock();
                pyRunning = false;
                threadSafety.unlock();

            }
        } 
    }

    void killPy() {
        PyErr_SetInterrupt();
    }

    void init() {
        help();

        Cacao::scheduleFunction(+[](){
            auto mtc = ThreadController::sharedState();
            CCDirector::sharedDirector()
                        ->getScheduler()
                        ->scheduleUpdateForTarget(mtc,1,false);
        });
        //pthread_t engine;
        //pthread_create(&engine, NULL, engineThread, NULL);
        std::thread(engineThread).detach();

        #if __APPLE__
        std::thread([](){
            CFRunLoopRef runloop = (CFRunLoopRef)CFRunLoopGetCurrent();

            CGEventMask interestedEvents = CGEventMaskBit(kCGEventFlagsChanged);
            CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 
                kCGEventTapOptionListenOnly,
                interestedEvents, +[] (CGEventTapProxy proxy, 
                                 CGEventType type, 
                                 CGEventRef event, 
                                 void* refcon) {

                if (type == kCGEventTapDisabledByTimeout || type == kCGEventTapDisabledByUserInput) {
                    CGEventTapEnable(stopRef,true);
                    return event;
                } else if(type == kCGEventFlagsChanged) {
                   // printf("flag change\n");
                    CGEventFlags flagsP = CGEventGetFlags(event);
                    bool shift = (flagsP & kCGEventFlagMaskShift) == kCGEventFlagMaskShift;
                    bool ctrl = (flagsP & kCGEventFlagMaskControl) == kCGEventFlagMaskControl || (flagsP & kCGEventFlagMaskCommand) == kCGEventFlagMaskCommand;

                    if (shift && ctrl) {
                        if (!haltOn) {
                            printf("holy shit they be doin it\n");
                            haltOn = true;
                            timeSinceOn = time(NULL);
                        }
                    } else {
                        haltOn = false;
                    }
                }

                // send event to next application
                return event;
            }, NULL);
            // by passing self as last argument, you can later send events to this class instance
            printf("its %p\n", eventTap);
            stopRef = eventTap;
            CFRunLoopSourceRef source = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, 
                                                                       eventTap, 0);

            CFRunLoopAddSource((CFRunLoopRef)runloop, source, kCFRunLoopCommonModes);

            CGEventTapEnable(eventTap, true);
            CFRunLoopRun();
            printf("what how\n");
        }).detach();
        #else
        std::thread([](){
            //shush
            volatile bool cum = true;
            while (cum) {
                bool shift = GetAsyncKeyState(VK_SHIFT) & 0x8000;
                bool ctrl = GetAsyncKeyState(VK_CONTROL) & 0x8000;

                if (shift && ctrl) {
                    if (!haltOn) {
                        printf("holy shit they be doin it\n");
                        haltOn = true;
                        timeSinceOn = time(NULL);
                    }
                } else {
                    haltOn = false;
                }
            }
        }).detach();
        #endif

        std::thread([](){
            for(;;) {
                if (haltOn && time(NULL) - timeSinceOn >= 5) {
                    threadSafety.lock();

                    if (pyRunning) {
                        //auto gstate = PyGILState_Ensure();
                        //PyThreadState_SetAsyncExc((uint64_t)ThreadController::sharedState()->threadID, PyExc_Exception);
                        //PyGILState_Release(gstate);

                        killPy();

                        Cacao::scheduleFunction(+[](){FLAlertLayer::create(NULL, "Emergency Stop", "Ok", NULL, "Successfully terminated script")->show();});
                        haltOn = false;
                    }

                    threadSafety.unlock();
                }
            }
        }).detach();
    }
    bool isRunning() {
        threadSafety.lock();
        bool out = pyRunning;
        threadSafety.unlock();
        return out;
    }
    bool runFile(char const* progname, std::string keybind) {
        FILE* fp = fopen(progname, "r");
        if (!fp){
            printf("ur file failed %s\n", progname);
            return false; //"Cant open file";
        }
        fclose(fp);

        engineQueue.push_back({std::string(progname), keybind});
        return true;
    }
}
