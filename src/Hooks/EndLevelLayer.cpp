#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include "../SillyPopup.hpp"

using namespace geode::prelude;

class $modify (EndLevelLayerExt, EndLevelLayer)
{
    void onSillyButton(CCObject*)
    {
        if (auto popup = SillyPopup::create())
        {
            popup->show();
        }
    }

    virtual void customSetup()
    {
        EndLevelLayer::customSetup();

        auto menu = CCMenu::create();   
        menu->setPosition(ccp(40, m_listLayer->getContentHeight() / 2 + (SillyBot::get()->hasBottedRun ? 0 : 15)));

        auto btn = CCMenuItemSpriteExtra::create(CircleButtonSprite::create(CCSprite::create("button-icon.png"_spr), CircleBaseColor::Green), this, menu_selector(EndLevelLayerExt::onSillyButton));

        menu->addChild(btn);
        m_listLayer->addChild(menu);
    }
};