#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// shut up cocos
#pragma warning(push, 0)
#include <cocos2d.h>
#pragma warning(pop)
#include <MinHook.h>
#include <gd.h>
using namespace gd;
using namespace cocos2d;
namespace Cacao {
	typedef void (cocos2d::CCObject::* CC_SEL)(cocos2d::CCObject*);
	typedef void (cocos2d::CCObject::* CC_SCHED)(float);

	inline cocos2d::CCPoint relativePosition(double x, double y) {
	    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
	    float xp = winSize.width * (x/100.);
	    float yp = winSize.height * (y/100.);
	    CCPoint ccp;
	    ccp.x = xp;
	    ccp.y = yp;
	    return ccp;
	}
	inline cocos2d::CCPoint addedPosition(double x, double y) {
	    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
	    float xp = winSize.width/2 + x;
	    float yp = winSize.height/2 + y;
	    CCPoint ccp;
	    ccp.x = xp;
	    ccp.y = yp;
	    return ccp;
	}

	inline std::vector<int> collapseGroups(GameObject* object) {
	     std::vector<int> gs;

	    int gid = 0;
	    int iter = 0;
	    do {
	        gid = object->getGroupID(iter);
	        iter++;
	        if (gid!=0)
	            gs.push_back(gid);
	    } while(gid!=0);
	    return gs;
	}

	template <typename K>
	inline void scheduleFunction(K func) {
	    GameManager::sharedState()->getScheduler()->scheduleSelector(reinterpret_cast<CC_SCHED&>(func), GameManager::sharedState(), 0.0, 0, 0.0, false);
	}

	class CacAlertLayer : public FLAlertLayer {
	 public:
	    CacAlertLayer() : 
	        FLAlertLayer() {
	            m_title = cocos2d::CCLabelBMFont::create("", "goldFont.fnt");
	        }

	    virtual void onClose() {}
	    virtual bool alertInit() {return true;}
	    virtual void keyBackClicked() {onClose();setKeypadEnabled(false);setTouchEnabled(false);removeFromParentAndCleanup(true);}
	    virtual void show() {
	    	registerWithTouchDispatcher();
	    	CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);

	    	setTouchEnabled(true);
	    	setKeypadEnabled(true);

	    	FLAlertLayer::show();
	    }

	    inline void l_onClose(CCObject* sender) {keyBackClicked();}

	    inline bool init(cocos2d::CCSize size) {
	    	if (CCLayerColor::initWithColor(ccc4(0,0,0,150))) { 

	    	    m_pLayer = CCLayer::create();
	    	    m_pButtonMenu = CCMenu::create();
	    	    addChild(m_pLayer);
	    	    m_pLayer->addChild(m_pButtonMenu);

	    	    auto scale9 = extension::CCScale9Sprite::create("GJ_square01.png");
	    	    scale9->setContentSize(size);
	    	    scale9->setPosition(Cacao::relativePosition(50,50));
	    	    m_pLayer->addChild(scale9, -2);


	    	    auto close = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");

	    	    m_closeButton = CCMenuItemSpriteExtra::create(close, this, menu_selector(CacAlertLayer::l_onClose));
	    	    m_closeButton->setPosition( - size.width / 2, size.height / 2 );
	    	    m_pButtonMenu->addChild(m_closeButton);


	    	    m_title->limitLabelWidth(size.width*4, .75f, .2f);
	    	    m_title->setPosition(Cacao::addedPosition(0, size.height/2 - 25));
	    	    m_pLayer->addChild(m_title);

	    	    alertInit();
	    	    return true;
	    	}
	    	return false;
	    }
	    inline bool init() {return init(cocos2d::CCSize(300.0,200.0));}
	    inline CacAlertLayer* title(char const* t) {m_title->setString(t, true);return this;}
	 protected:
	    CCMenuItemSpriteExtra* m_closeButton;
	    cocos2d::CCLabelBMFont* m_title;
	};
};
#define GM GameManager::sharedState()
#define LEL GM->getEditorLayer()
