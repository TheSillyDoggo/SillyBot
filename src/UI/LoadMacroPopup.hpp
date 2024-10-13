#pragma once

#include <Geode/Geode.hpp>
#include "MacroCell.hpp"
#include "../Bot.hpp"

using namespace geode::prelude;

class LoadMacroPopup : public GJDropDownLayer
{
    public:
        CCLayer* layer;
        TextArea* error;
        std::vector<Ref<MacroCell>> cells;
        ScrollLayer* scroll;
        std::filesystem::path path;

        virtual void customSetup();

        void refreshList();
        void onRefresh(CCObject* sender);
        void onFolder(CCObject* sender);

        static LoadMacroPopup* create();
};