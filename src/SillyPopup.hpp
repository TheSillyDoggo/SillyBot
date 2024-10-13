
#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "SillyBot.hpp"

using namespace geode::prelude;

class SillyPopup : public geode::Popup<>
{
    public:
        CCLabelBMFont* macroInfo = nullptr;
        CCMenuItemToggler* disable = nullptr;
        CCMenuItemToggler* record = nullptr;
        CCMenuItemToggler* playback = nullptr;

        virtual bool setup();

        void onClose(CCObject* sender);
        void keyBackClicked();

        void updateMacroInfo();
        void onStateChanged();

        void onSettings(CCObject*);
        void onLoad(CCObject*);
        void onSave(CCObject*);
        void onChangeState(CCObject*);
        
        void setupSettings(CCScale9Sprite* sprite);

        static SillyPopup* create();
        static SillyPopup* get();
};