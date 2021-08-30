#include <engine.h>
#include <Python.h>
#include <thread_control.h>
#include <Cacao.hpp>
#include <Cocoa/Cocoa.h>
#include <vector>
#include <helper_hooks.h>

using namespace cocos2d;
#include "cy/main.h"

static PyObject* PyGD_alert(PyObject *self, PyObject *args) {
    char* title;
    char* des;
    char* button;
    if(!PyArg_ParseTuple(args, "sss:alert", &title, &des, &button)) // get the arg
        return NULL;

    dispatch_async(dispatch_get_main_queue(), ^{
        auto fl = FLAlertLayer::create(title, std::string(des), button);
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
    static CFMachPortRef stopRef;
    static bool haltOn = false;
    static uint32_t timeSinceOn = 0;
    std::vector<std::string> engineQueue;
    static NSLock* threadSafety = nullptr;
    static bool pyRunning;

    void* engineThread(void*) {
        if (!threadSafety) {
            threadSafety = [[NSLock alloc] init];
            PyImport_AppendInittab("pygd", &PyInit_PyGD);
            PyImport_AppendInittab("EditorUI", &PyInit_main);

            Py_Initialize();
            PyEval_InitThreads(); 
        }

        pyThreadID = (uint64_t)pthread_self();
        while (true) {
            if (engineQueue.size() > 0) {

                [threadSafety lock];
                pyRunning = true;
                [threadSafety unlock];

                auto file = engineQueue.back().c_str();
                engineQueue.pop_back();
                
                FILE* fp = fopen(file, "r");
                if (!fp)
                    continue;
                PyRun_SimpleString("locals().clear()\nfrom EditorUI import *\nimport sys;__import__(\"time\").sleep(0.1);sys.path.append('/Users/jakrillis/projects/3dmodel2gd')");

                PyRun_SimpleFile(fp, file);
                printf("finished running file\n");
                fclose(fp);

                [threadSafety lock];
                pyRunning = false;
                [threadSafety unlock];

            }
        } 
        return NULL;
    }

    CGEventRef keyCallback(CGEventTapProxy proxy, 
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
    }

    void killPy() {
        pthread_kill((pthread_t)pyThreadID, SIGINT);
        pthread_kill(ThreadController::sharedState()->threadID, SIGINT);

        /*while (!pthread_kill( (pthread_t)pyThreadID, 0 )) {

        }*/
        pthread_t engine;
        pthread_create(&engine, NULL, engineThread, NULL);
    }

    void init() {
        help();
        auto pyQueue = dispatch_queue_create("python", DISPATCH_QUEUE_CONCURRENT);
        auto keyboardQueue = dispatch_queue_create("halter_keyboard", DISPATCH_QUEUE_CONCURRENT);
        auto haltQueue = dispatch_queue_create("halter_main", DISPATCH_QUEUE_CONCURRENT);

        dispatch_async(dispatch_get_main_queue(), ^{
            auto mtc = ThreadController::sharedState();
            CCDirector::sharedDirector()
                        ->getScheduler()
                        ->scheduleUpdateForTarget(mtc,1,false);
        });
        pthread_t engine;
        pthread_create(&engine, NULL, engineThread, NULL);

        dispatch_async(keyboardQueue, ^{
            CFRunLoopRef runloop = (CFRunLoopRef)CFRunLoopGetCurrent();

            CGEventMask interestedEvents = CGEventMaskBit(kCGEventFlagsChanged);
            CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 
                kCGEventTapOptionListenOnly,
                interestedEvents, keyCallback, NULL);
            // by passing self as last argument, you can later send events to this class instance
            printf("its %p\n", eventTap);
            stopRef = eventTap;
            CFRunLoopSourceRef source = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, 
                                                                       eventTap, 0);

            CFRunLoopAddSource((CFRunLoopRef)runloop, source, kCFRunLoopCommonModes);

            CGEventTapEnable(eventTap, true);
            CFRunLoopRun();
            printf("what how\n");
        });

        dispatch_async(haltQueue, ^{
            for(;;) {
                if (haltOn && time(NULL) - timeSinceOn >= 5) {
                    [threadSafety lock];

                    if (pyRunning) {
                        //auto gstate = PyGILState_Ensure();
                        //PyThreadState_SetAsyncExc((uint64_t)ThreadController::sharedState()->threadID, PyExc_Exception);
                        //PyGILState_Release(gstate);

                        killPy();

                        Cacao::scheduleFunction(+[](){FLAlertLayer::create("Emergency Stop", "Successfully terminated script", "Ok")->show();});
                        haltOn = false;
                    }

                    [threadSafety unlock];
                }
            }
        });
    }
    bool isRunning() {
        [threadSafety lock];
        bool out = pyRunning;
        [threadSafety unlock];
        return out;
    }
    bool runFile(char const* progname) {
        FILE* fp = fopen(progname, "r");
        if (!fp){
            printf("ur file failed %s\n", progname);
            return false; //"Cant open file";
        }
        fclose(fp);

        engineQueue.push_back(std::string(progname));
        return true;
    }
}
