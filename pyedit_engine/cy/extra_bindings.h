// bad practice my ass
#include <Cacao.hpp>
using namespace cocos2d;

#include <cstdlib>
#include <memory>
#include <cxxabi.h>
#include <string>
#include <thread_control.h>
#include <helper_hooks.h>
#include <mutex>

inline CCPoint mouse_coords(0,0);
inline std::vector<event_base*> events;
inline std::mutex event_lock;

inline char const* demangle(const char* name) {

    int status = -4; // some arbitrary value to eliminate the compiler warning

    // enable c++11 by passing the flag -std=c++11 to g++
    auto res = abi::__cxa_demangle(name, NULL, NULL, &status);

    return (status==0) ? res : name ;
}

inline EditorUI* EditorUI_shared() {
	if (LEL) {
		return LEL->_editorUI();
	} else {
		return 0;
	}
}
inline long makeUsable(void* stuff) {
	return (long)stuff;
}


inline char* getNode(CCObject* node) {
    char* name = (char*)(demangle(typeid(*node).name()));
    return name;
}

inline bool onMainThread() {
	return ThreadController::sharedState()->onMain();
}

inline CCPoint getMouseCoords() {
	return mouse_coords;
}

inline event_base* popEvent() {
	event_lock.lock();
	event_base* ret;
	if (events.empty()) {
		ret = nullptr;
	} else {
		//printf("event %d\n", events.size());
		auto out = events.back();
		//printf("yeah its an event\n");

		events.pop_back();
		ret = out;
	}

	event_lock.unlock();
	
	return ret;
}

inline void clearEvents() {
	event_lock.lock();
	events.clear();
	event_lock.unlock();
}

inline std::vector<int> getObjGroups(GameObject* obj) {
	return Cacao::collapseGroups(obj);
}
