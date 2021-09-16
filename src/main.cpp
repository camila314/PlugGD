#if  __APPLE__
    #define CAC_PROJ_NAME "PoweredByPy"
    #include <CacKit>
#else
    #include <wind32.h>
#endif

#include <engine.h>
#include "ScriptLayer.h"
#include "ScriptController.h"

static bool _usable = false;

#if __APPLE__
class swag: public $EditorUI<swag> {
#else
class swag : public EditorUI {
 public:
    static void(*$setupCreateMenu)(swag*);
    static void(*$keyDown)(swag*, int);
#endif
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
        #if __APPLE__
            $EditorUI::setupCreateMenu();
        #else
            $setupCreateMenu(this);
        #endif

        auto menu = cocos2d::CCMenu::create();
        menu->setTag(2653);
        this->addChild(menu);

        #if __APPLE__ // gd.h is weird
        auto sprite = ButtonSprite::create("Scripts", 50, 0, 0.7, 0, "bigFont.fnt", "GJ_button_01.png", 30);
        #else
        auto sprite = ButtonSprite::create("Scripts", 50, false, "bigFont.fnt", "GJ_button_01.png", 30, 0.7);
        #endif

        #if __APPLE__
        auto mitem = CCMenuItemSpriteExtra::create(sprite, sprite, this, menu_selector(swag::onPress));
        #else
        auto mitem = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(swag::onPress));
        #endif
        mitem->setTag(8214);

        auto stopsprite = CCSprite::createWithSpriteFrameName("GJ_stopEditorBtn_001.png");

        #if __APPLE__
        auto mitem2 = CCMenuItemSpriteExtra::create(stopsprite, stopsprite, this, menu_selector(swag::onStop));
        #else
        auto mitem2 = CCMenuItemSpriteExtra::create(stopsprite, this, menu_selector(swag::onStop));
        #endif
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
        _usable = true;
    }

    void keyDown(cocos2d::enumKeyCodes key) {
        if (!_usable) {
            #if __APPLE__
                $EditorUI::keyDown(key);
            #else
                $keyDown(this, key);
            #endif
            return;
        }
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

                if (this->getChildByTag(2653)) {
                    this->getChildByTag(2653)->getChildByTag(8486)->setVisible(true);
                    this->getChildByTag(2653)->getChildByTag(8214)->setVisible(false);
                }
                //std::cout<<std::string(k)<<"\n";
                engine::runFile((scripter()->plugFolder() + v).c_str(), std::string(k));
                return;
            } 
        }
        #if __APPLE__
            $EditorUI::keyDown(key);
        #else
            $keyDown(this, key);
        #endif
    }
#if __APPLE__
} ya;
#else
};
void(*swag::$setupCreateMenu)(swag*) = nullptr;
void(*swag::$keyDown)(swag*, int) = nullptr;
#endif

#if __APPLE__
class $redirect(GameManager) {
#else
class swag_2 {
 public:
    static void(*$returnToLastScene)(swag_2*, GJGameLevel* c);
#endif
 public:
    void returnToLastScene(GJGameLevel* carry) {
        #if __APPLE__
        $GameManager::returnToLastScene(carry);
        #else
        $returnToLastScene(this, carry);
        #endif
        engine::killPy();
    }
#if __APPLE__
} ya2;
#else
};
void(*swag_2::$returnToLastScene)(swag_2*, GJGameLevel* c) = nullptr;
#endif

#if __APPLE__
$apply();

#else
void __fastcall idfk(void* dontcare, void* dontcare2) {
    scripter()->refreshFiles(false);
}
#endif

void inject() {
    #if _WIN32
        MH_Initialize();

        uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleA(0));

        auto tmp1 = &swag_2::returnToLastScene;
        MH_CreateHook(reinterpret_cast<LPVOID>(base+0xce6a0), reinterpret_cast<void*&>(tmp1), reinterpret_cast<void**>(&swag_2::$returnToLastScene));
        auto tmp2 = &swag::keyDown;
        MH_CreateHook(reinterpret_cast<LPVOID>(base+0x91a30), reinterpret_cast<void*&>(tmp2), reinterpret_cast<void**>(&swag::$keyDown));
        auto tmp3 = &swag::setupCreateMenu;
        MH_CreateHook(reinterpret_cast<LPVOID>(base+0x7caf0), reinterpret_cast<void*&>(tmp3), reinterpret_cast<void**>(&swag::$setupCreateMenu));
    #endif

    //engine::init();
    printf("injectd\n");
    //Cacao::scheduleFunction(+[](){scripter()->refreshFiles(false);});

    #if _WIN32
    printf("hookd, create menu is %p, base is %p\n", base+0x7caf0, base);
    MH_EnableHook(MH_ALL_HOOKS);
    Cacao::scheduleFunction(idfk);
    #endif
}

#if _WIN32
DWORD WINAPI thread_func(void* hModule) {
    AllocConsole();
    SetConsoleTitleA("PoweredByPy debug");
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

    inject();
    return true;
}
BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0x100, thread_func, handle, 0, 0);
    }
    return TRUE;
}
#endif