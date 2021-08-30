#pragma once

void help();

struct event_base {
	std::string name;
	//event_base(std::string n) : name(n) {}
};

event_base* threadSafePop();

struct add_object_event : event_base {
	int object_id;
};
