#pragma once

#include <ScriptController.h>

class ScriptLayer : public Cacao::CacAlertLayer {
 public:
 	ScriptLayer() : m_index(0) {}
	static ScriptLayer* create();
	bool init();
	void onSwitch(int newindex);
	void addEntry(int index, std::pair<Keybind, std::string> entry);
	inline void onLeft(CCObject*) {onSwitch(m_index-1);}
	inline void onRight(CCObject*) {onSwitch(m_index+1);}
	void onOpen(CCObject*);
 protected:
  	int m_index;
  	CCMenuItemSpriteExtra* m_rightBtn;
  	CCMenuItemSpriteExtra* m_leftBtn;
  	std::vector<CCNode*> m_entries;
};