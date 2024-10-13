#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../UI/ReplayManagerDropdown.hpp"
#include "../SillyBot.hpp"

using namespace geode::prelude;

class $modify (PauseLayerExt, PauseLayer)
{
    struct Fields
    {
        CircleButtonSprite* btn;
    };

    void onReplayBtn(CCObject* sender);
    void updateButtonState();

    virtual void customSetup();

    static PauseLayerExt* get();
};