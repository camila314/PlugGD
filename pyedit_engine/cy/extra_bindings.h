// bad practice my ass
#if __APPLE__
#include <Cacao.hpp>
using namespace cocos2d;
#else
#include <wind32.h>
#endif

#include <cstdlib>
#include <memory>
#include <string>
#include <thread_control.h>
#include <helper_hooks.h>
#include <mutex>

#if __APPLE__
#include <cxxabi.h>
#endif

inline std::vector<event_base*> events;
inline std::mutex event_lock;


inline int& gob_getZOrder(GameObject* gob) {
	#if __APPLE__
	return gob->_zOrder();
	#else
	return gob->m_nGameZOrder;
	#endif
}
inline int gob_getID(GameObject* gob) { 
	#if __APPLE__
	return gob->_id();
	#else
	return gob->m_nObjectID;
	#endif
}

inline GJSpriteColor* getRelSpriteColor(GameObject* g, int k) {
	#if __APPLE__
	return g->getRelativeSpriteColor(k);
	#else
	uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleA(0));
	auto fptr = reinterpret_cast<GJSpriteColor*(__thiscall *)(GameObject*, int)>(base+0xef520);
	return fptr(g, k);
	#endif
}


inline LevelEditorLayer* getEditorLayer(EditorUI* eui) {
	#if __APPLE__
	return eui->_editorLayer();
	#else
	return eui->m_pEditorLayer;
	#endif
}

inline char const* demangle(const char* name) {
	#if __APPLE__
    int status = -4; // some arbitrary value to eliminate the compiler warning

    // enable c++11 by passing the flag -std=c++11 to g++
    auto res = abi::__cxa_demangle(name, NULL, NULL, &status);

    return (status==0) ? res : name ;
    #else
    std::string sn(name);

    sn.erase(sn.find("class "), 6);
    return sn.c_str();
    #endif
}

inline EditorUI* EditorUI_shared() {
	if (LEL) {
		#if __APPLE__
		return LEL->_editorUI();
		#else
		return LEL->getEditorUI();
		#endif
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
