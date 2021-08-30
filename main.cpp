#define CAC_PROJ_NAME "pyedit"
#include <CacKit>
#include <engine.h>
#include "ScriptLayer.h"
#include "ScriptController.h"

class swag: public $EditorUI<swag> {
 public:
	void onPress(CCObject* sender) {
		//engine::runFile("/Users/jakrillis/t.py");
		if (scripter()->refreshFiles(true)) 
			ScriptLayer::create()->show();
	}
	void onStop(CCObject* sender) {
		engine::killPy();
		this->getChildByTag(2653)->getChildByTag(8486)->setVisible(false);
		this->getChildByTag(2653)->getChildByTag(8214)->setVisible(true);
	}
	void setupCreateMenu() {
	    $EditorUI::setupCreateMenu();

	    auto menu = cocos2d::CCMenu::create();
	    menu->setTag(2653);
	    this->addChild(menu);

	    auto sprite = ButtonSprite::create("Scripts", 50, 0, 0.7, 0, "bigFont.fnt", "GJ_button_01.png", 30);
	    auto mitem = CCMenuItemSpriteExtra::create(sprite, sprite, this, menu_selector(swag::onPress));
	    mitem->setTag(8214);

	    auto stopsprite = CCSprite::createWithSpriteFrameName("GJ_stopEditorBtn_001.png");
	    auto mitem2 = CCMenuItemSpriteExtra::create(stopsprite, stopsprite, this, menu_selector(swag::onStop));
	    mitem2->setVisible(false);
	    mitem2->setTag(8486);

	    menu->addChild(mitem);
	    menu->addChild(mitem2);

	    auto dir = CCDirector::sharedDirector();
	    float top = dir->getScreenTop()-19.5;
	    float rgt = dir->getScreenRight()-124.5;
	    CCPoint base = ccp(rgt, top);
	    menu->setPosition(base);

	    auto lam = +[](EditorUI* slf){
	    	if (!engine::isRunning()) {
	    		slf->getChildByTag(2653)->getChildByTag(8486)->setVisible(false);
	    		slf->getChildByTag(2653)->getChildByTag(8214)->setVisible(true);
	    	} else {
	    		slf->getChildByTag(2653)->getChildByTag(8486)->setVisible(true);
	    		slf->getChildByTag(2653)->getChildByTag(8214)->setVisible(false);
	    	}
	    };
	    getScheduler()->scheduleSelector(reinterpret_cast<Cacao::CC_SCHED&>(lam), this, 0.0, -1, 0.0, false);
	}

	void keyDown(cocos2d::enumKeyCodes key) override {
		auto dispatcher = CCDirector::sharedDirector()->getKeyboardDispatcher();

		Keybind justClick;
		justClick.exists = true;
		justClick.shift = dispatcher->getShiftKeyPressed();
		justClick.ctrl = dispatcher->getControlKeyPressed();
		justClick.alt = dispatcher->getAltKeyPressed();
		justClick.key = key;

		for (auto [k, v] : scripter()->getKeybinds()) {
			if (k.shift != dispatcher->getShiftKeyPressed())
				continue;
			if (k.alt != dispatcher->getAltKeyPressed())
				continue;
			if (k.ctrl != dispatcher->getControlKeyPressed())
				continue;

			if (k.key == key) {
				printf("running %s\n", v.c_str());

				this->getChildByTag(2653)->getChildByTag(8486)->setVisible(true);
				this->getChildByTag(2653)->getChildByTag(8214)->setVisible(false);

				engine::runFile((scripter()->plugFolder() + v).c_str());
				return;
			} 
		}
		$EditorUI::keyDown(key);
	}
} ya;


$apply();

void injec() __attribute__((constructor));

void injec() {
	engine::init();
	printf("injectd\n");
	//Cacao::repeatFunction(+[](){scripter()->refreshFiles(false);}, -1, 5.0);
	Cacao::scheduleFunction(+[](){scripter()->refreshFiles(false);});
}