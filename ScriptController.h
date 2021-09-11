#pragma once

#include <Cacao.hpp>
using namespace cocos2d;

struct Keybind {
	char key;
	bool shift;
	bool alt;
	bool ctrl;
	bool exists;

	static Keybind fromArray(std::vector<std::string> v);
	static std::map<std::string, enumKeyCodes> customKeys;
	operator std::string() const;
	inline bool operator<(const Keybind& b) const {return reinterpret_cast<const uint64_t&>(*this)<reinterpret_cast<const uint64_t&>(b);}
};

class ScriptController : public cocos2d::CCNode {
 public:
 	ScriptController() {
 	}
 	inline static ScriptController* shared() {
 		if (!ScriptController::_shared) {
 			ScriptController::_shared = new ScriptController();
 		}

 		return ScriptController::_shared;
 	}

 	bool refreshFiles(bool err);
 	std::string plugFolder();
 	inline std::map<Keybind, std::string> getKeybinds() {return m_keybinds;}

 protected:

 	static ScriptController* _shared;

 	std::map<Keybind, std::string> m_keybinds;
};

inline ScriptController* scripter() {return ScriptController::shared();}