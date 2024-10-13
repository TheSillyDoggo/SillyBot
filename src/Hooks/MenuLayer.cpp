/*#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../UI/ReplayManagerDropdown.hpp"
#include "../Recorder/VideoRecorder.hpp"

using namespace geode::prelude;

class $modify (MenuLayerExt, MenuLayer)
{
    void onReplayManager(CCObject*)
    {
        this->addChild(VideoRecorder::createWithPlayLayer(LevelTools::getLevel(22, false)), 69);
        //ReplayManagerDropdown::create()->showLayer(false);
    }

    bool init()
    {
        if (!MenuLayer::init())
            return false;

        if (auto menu = getChildByID("bottom-menu"))
        {
            auto btn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_everyplayBtn_001.png"), this, menu_selector(MenuLayerExt::onReplayManager));
            btn->setZOrder(6);
            btn->setID("replay-button"_spr);

            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }
};*/