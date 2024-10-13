#pragma once

#include <Geode/Geode.hpp>
#include "MacroCell.hpp"
#include "../Bot.hpp"

using namespace geode::prelude;

class SaveMacroPopup : public geode::Popup<>, public TextInputDelegate
{
    public:
        std::filesystem::path path;
        TextInput* input;
        CCMenuItemToggler* gdr;
        CCMenuItemToggler* gdrJson;
        CCMenuItemSpriteExtra* saveBtn;
        CCLabelBMFont* error;
        CCScale9Sprite* errorBG;
        bool jsonSelected;

        virtual bool setup();
        virtual void textChanged(CCTextInputNode* input);

        void updateSelectedType(bool json);

        void onGDR(CCObject*);
        void onGDRJson(CCObject*);
        void onSave(CCObject*);

        void updateInfo();

        static SaveMacroPopup* create();
};