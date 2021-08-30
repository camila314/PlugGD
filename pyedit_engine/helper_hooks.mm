#include <Cacao.hpp>
#include <cy/extra_bindings.h>
#include <Cocoa/Cocoa.h>

ModContainer* _hnm;
NSLock* evlock;

void _mousemove(id self, SEL _cmd, NSEvent* ev) {
	FCAST(_mousemove, _hnm->getOriginal(getBase()+0x3b0f90))(self, _cmd, ev);

	mouse_coords = ccp([ev locationInWindow].x, [ev locationInWindow].y);
}

void _onCreate(EditorUI* self, int _id) {
	FCAST(_onCreate, _hnm->getOriginal(getBase()+0x200d0))(self, _id);
	auto ev = new add_object_event;
	ev->name = "add_object";
	ev->object_id = _id;
	[evlock lock];
	events.push_back(ev);
	[evlock unlock];
}


event_base* threadSafePop() {
	event_base* ret;
	[evlock lock];
	if (events.empty()) {
		ret = nullptr;
	} else {
		//printf("event %d\n", events.size());
		auto out = events.back();
		//printf("yeah its an event\n");

		events.pop_back();
		ret = out;
	}
	[evlock unlock];
	return ret;
}

void help() {
	evlock = [[NSLock alloc] init];
	_hnm = new ModContainer("helper pygd");
	_hnm->registerHook(getBase()+0x3b0f90, _mousemove);
	_hnm->registerHook(getBase()+0x200d0, _onCreate);
	_hnm->enable();
}