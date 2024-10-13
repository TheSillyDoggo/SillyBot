#include "MacroCell.hpp"

bool MacroCell::init(bool online, std::string path, CCSize size, int i, bool showSelect)
{
    if (!CCLayerColor::init())
        return false;

    this->setContentSize(size);
    this->setAnchorPoint(ccp(0, 0.5f));
    this->setColor(i % 2 == 0 ? ccc3(52.5f, 52.5f, 52.5f) : ccc3(35, 35, 35));
    this->setOpacity(255);
    this->showSelect = showSelect;
    this->path = path;
    
    loadMenu = CCMenu::create();
    loadMenu->setVisible(false);

    auto loadSpr = ButtonSprite::create("Load", 60, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_01.png", 30);
    loadSpr->m_label->setScale(loadSpr->m_label->getScale() * 0.8f);

    auto loadBtn = CCMenuItemSpriteExtra::create(loadSpr, this, menu_selector(MacroCell::onSelect));
    loadMenu->addChild(loadBtn);

    loadSpr->setScale(0.85f);

    circle = LoadingCircleSprite::create();
    circle->setScale((size.height - 10) / circle->getContentHeight());

    name = CCLabelBMFont::create("", "bigFont.fnt");
    name->setAnchorPoint(ccp(0, 1));

    info = CCLabelBMFont::create("", "chatFont.fnt");
    info->setAnchorPoint(ccp(0, 0));
    info->setOpacity(100);

    if (Mod::get()->getSettingValue<bool>("threaded-loading"))
    {
        std::thread([this, online, path]{
            if (!online)
                macro = SillyBot::get()->getMacro(path, false);

            Loader::get()->queueInMainThread([this]{
                loadMacroInfo();
            });
        }).detach();
    }
    else
    {
        if (!online)
            macro = SillyBot::get()->getMacro(path, false);

        loadMacroInfo();
    }

    this->addChildAtPosition(circle, Anchor::Center);
    this->addChildAtPosition(loadMenu, Anchor::Right, ccp(-42, 0));
    this->addChildAtPosition(name, Anchor::TopLeft, ccp(6.5f, -2));
    this->addChildAtPosition(info, Anchor::BottomLeft, ccp(6.5f, 2));
    
    return true;
}

void MacroCell::loadMacroInfo()
{
    circle->setVisible(false);
    loadMenu->setVisible(showSelect);

    name->setString(getFileNameFromPath(path).c_str());
    name->limitLabelWidth(this->getContentWidth() / 2, 0.6f, 0.1f);

    if (macro.levelInfo.id == 0)
        info->setString(fmt::format("Name: {}", macro.levelInfo.name).c_str());
    else
        info->setString(fmt::format("ID: {}  Name: {}", macro.levelInfo.id, macro.levelInfo.name).c_str());
    
    info->limitLabelWidth(this->getContentWidth() - 100, 0.6f, 0.1f);
}

void MacroCell::onSelect(CCObject* sender)
{
    if (macro.botInfo.name != "SillyBot")
    {
        geode::createQuickPopup(
            "Warning",
            fmt::format("This <cs>macro</c> was made for a different\n<ca>replay</c> bot (<cy>{}</c>).\nThis can cause <cr>issues with accuracy</c>.\nAre you <cl>sure</c> you want to load the <cs>macro</c>?", macro.botInfo.name),
            "Cancel",
            "Load",
            [this](FLAlertLayer* alert, bool right)
            {
                if (right)
                {
                    load();
                }
            }
        );

        return;
    }

    load();
}

void MacroCell::load()
{
    SillyBot::get()->loadMacroFromFile(path);

    Loader::get()->queueInMainThread([this]{
        Loader::get()->queueInMainThread([this]{
            auto alert = TextAlertPopup::create("Loaded Macro", 0.5f, 0.6f, 150, "");
            alert->setPositionY(50);
            CCScene::get()->addChild(alert, 9999999);

            layerToDelete->exitLayer(nullptr);
        });
    });
}

std::string MacroCell::getFileNameFromPath(std::filesystem::path path)
{
    if (path.has_stem())
        path = path.stem();

    if (path.has_stem())
        path = path.stem();

    return path.string();
}

MacroCell* MacroCell::create(bool online, std::string path, CCSize size, int i, bool showSelect)
{
    auto pRet = new MacroCell();

    if (pRet && pRet->init(online, path, size, i, showSelect))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}