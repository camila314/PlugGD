#if __APPLE__
#include <Cacao.hpp>
#else
#include <wind32.h>
#endif

#include <cy/extra_bindings.h>


#if __APPLE__
void(*orig_onCreate)(EditorUI*, int);
ModContainer* _hnm;
#else
void(__thiscall *orig_onCreate)(EditorUI*, int);
#endif

#if __APPLE__
void _onCreate(EditorUI* self, int _id)
#else
void __fastcall _onCreate(EditorUI* self, int dummy, int _id)
#endif
{
	orig_onCreate(self, _id);
	auto ev = new add_object_event;
	ev->name = "add_object";
	ev->object_id = _id;
	event_lock.lock();	
	events.push_back(ev);
	event_lock.unlock();
}


void help() {
	#if __APPLE__
	_hnm = new ModContainer("helper pygd");
	orig_onCreate = _hnm->registerHook(getBase()+0x200d0, _onCreate);
	_hnm->enable();
	#else
	MH_CreateHook(GetModuleHandle(0)+0x85750, (void*)_onCreate, (void**)&orig_onCreate);
	#endif
}