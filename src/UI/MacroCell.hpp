#pragma once

#include <Geode/Geode.hpp>
#include "../Bot.hpp"
#include "../SillyBot.hpp"

using namespace geode::prelude;

class MacroCell : public CCLayerColor
{
    public:
        SillyMacro macro;
        std::string path;
        LoadingCircleSprite* circle;
        CCMenu* loadMenu;
        CCLabelBMFont* name;
        CCLabelBMFont* info;
        bool showSelect;
        GJDropDownLayer* layerToDelete;

        bool init(bool online, std::string path, CCSize size, int i, bool showSelect);

        void onSelect(CCObject* sender);
        
        void load();
        void loadMacroInfo();
        std::string getFileNameFromPath(std::filesystem::path path);

        static MacroCell* create(bool online, std::string path, CCSize size, int i, bool showSelect = false);
};