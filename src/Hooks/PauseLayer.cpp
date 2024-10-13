#include "PauseLayer.hpp"
#include "../SillyPopup.hpp"

Ref<PauseLayerExt> instance;

void PauseLayerExt::customSetup()
{
    PauseLayer::customSetup();
    instance = this;

    if (auto menu = getChildByID("left-button-menu"))
    {
        menu->setPositionX(menu->getPositionX() + 5);

        m_fields->btn = CircleButtonSprite::create(CCSprite::create("button-icon.png"_spr), CircleBaseColor::Blue);

        auto button = CCMenuItemSpriteExtra::create(m_fields->btn, this, menu_selector(PauseLayerExt::onReplayBtn));

        menu->addChild(button);
        menu->updateLayout();
        updateButtonState();
    }
}

void PauseLayerExt::updateButtonState()
{
    if (m_fields->btn)
    {
        auto bot = SillyBot::get();
        auto col = bot->colourFromStatus();

        auto framename = fmt::format("geode.loader/baseCircle_{}_{}.png", baseEnumToString(CircleBaseSize::Medium), baseEnumToString(col));
        auto frame = CCSpriteFrameCache::get()->spriteFrameByName(framename.c_str());

        m_fields->btn->setDisplayFrame(frame);
    }
}

void PauseLayerExt::onReplayBtn(CCObject* sender)
{
    if (auto popup = SillyPopup::create())
    {
        popup->show();
    }
}

PauseLayerExt* PauseLayerExt::get()
{
    return instance;
}