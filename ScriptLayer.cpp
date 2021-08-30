#include <Cacao.hpp>
#include "ScriptLayer.h"

using namespace cocos2d;

bool ScriptLayer::init() {
	if (Cacao::CacAlertLayer::init(CCSizeMake(300, 200))) {
		m_title->setString("Script Keybinds");

		auto rightSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
		rightSprite->setFlipX(true);
		m_rightBtn = CCMenuItemSpriteExtra::create(rightSprite, rightSprite, this, menu_selector(ScriptLayer::onRight));
		m_rightBtn->setPosition(ccp(175, 0));
		m_rightBtn->setVisible(false);
		m_buttonMenu->addChild(m_rightBtn);

		auto leftSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
		m_leftBtn = CCMenuItemSpriteExtra::create(leftSprite, leftSprite, this, menu_selector(ScriptLayer::onLeft));
		m_leftBtn->setPosition(ccp(-175, 0));
		m_leftBtn->setVisible(false);
		m_buttonMenu->addChild(m_leftBtn);

		auto openSprite = CCSprite::createWithSpriteFrameName("GJ_shareBtn_001.png");
		openSprite->setScale(0.5);
		auto mitem = CCMenuItemSpriteExtra::create(openSprite, openSprite, this, menu_selector(ScriptLayer::onOpen));
		mitem->setPosition(ccp(105.2, 72.8));
		m_buttonMenu->addChild(mitem);

		onSwitch(0);
		return true;
	}
	return false;
}

void ScriptLayer::onSwitch(int newindex) {

	for (auto i : m_entries) {
		i->removeFromParentAndCleanup(true);
	}
	m_entries.clear();

	m_index = newindex;

	int i = 0;
	for (auto p : scripter()->getKeybinds()) {
		addEntry(i++, p);
	}

	if ((m_index+1)*6 < m_entries.size())
		m_rightBtn->setVisible(true);
	else
		m_rightBtn->setVisible(false);

	if (m_index > 0)
		m_leftBtn->setVisible(true);
	else
		m_leftBtn->setVisible(false);
}
void ScriptLayer::addEntry(int ind, std::pair<Keybind, std::string> entry) {
	if ((m_index+1) * 6 <= ind || m_index * 6 > ind) return;

	auto entry_node = CCNode::create();
	m_mainLayer->addChild(entry_node);
	entry_node->setPosition(Cacao::addedPosition(0, 40) + ccp(0, -20 * (ind%6)));

	auto box = CCLayerColor::create(ccc4(0,0,0,ind%2==0 ? 100 : 140), 250, 20);//cocos2d::extension::CCScale9Sprite::create("square02b_small.png");
	box->setPosition(ccp(-250.0/2, -20/2.0));
	entry_node->addChild(box);

	auto bind = CCLabelBMFont::create(std::string(entry.first).c_str(), "goldFont.fnt");
	bind->setScale(0.6);
	bind->setPosition(ccp(-115,0));
	bind->setAnchorPoint(ccp(0,0.5));
	entry_node->addChild(bind);


	auto script = CCLabelBMFont::create(entry.second.c_str(), "bigFont.fnt");
	script->setScale(0.45);
	script->setPosition(ccp(115,0));
	script->setAnchorPoint(ccp(1,0.5));
	entry_node->addChild(script);

	m_entries.push_back(entry_node);
}

void ScriptLayer::onOpen(CCObject* o) {
	#ifdef _WIN32
		system((std::string("explorer ") + scripter()->plugFolder()).c_str());
	#else
		system((std::string("open \"") + scripter()->plugFolder() + std::string("\"")).c_str());
	#endif
}

ScriptLayer* ScriptLayer::create() {
	auto pRet = new ScriptLayer();
	if (pRet && pRet->init()) {
	    pRet->autorelease();
	    return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
