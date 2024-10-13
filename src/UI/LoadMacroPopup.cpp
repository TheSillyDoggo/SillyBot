#include "LoadMacroPopup.hpp"

void LoadMacroPopup::customSetup()
{
    path = Mod::get()->getConfigDir() / "macros/";

    if(!std::filesystem::exists(path))
        std::filesystem::create_directory(path);

    layer = CCLayer::create();
    layer->ignoreAnchorPointForPosition(false);
    layer->setPosition(CCDirector::get()->getWinSize() / 2);
    m_mainLayer->addChild(layer);

    auto menu = CCMenu::create();

    auto refresh = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"), this, menu_selector(LoadMacroPopup::onRefresh));
    refresh->setPosition(ccp(m_listLayer->getContentWidth() / 2 + (20 / 4), -m_listLayer->getContentHeight() / 2 - 10));
    menu->addChild(refresh);

    auto folder = CCMenuItemSpriteExtra::create(CircleButtonSprite::createWithSpriteFrameName("folderIcon_001.png", 1, CircleBaseColor::Green), this, menu_selector(LoadMacroPopup::onFolder));
    folder->setPosition(ccp(-m_listLayer->getContentWidth() / 2 - (20 / 4), -m_listLayer->getContentHeight() / 2 - 10));
    menu->addChild(folder);

    scroll = ScrollLayer::create(m_listLayer->getContentSize() + ccp(0, -5));
    scroll->setPositionY(5);

    error = TextArea::create("", "bigFont.fnt", 1, 1000, ccp(0.5f, 0.5f), 10, false);
    error->setScale(0.65f);

    refreshList();
    scroll->moveToTop();

    m_listLayer->addChild(scroll);
    layer->addChildAtPosition(error, Anchor::Center);
    layer->addChildAtPosition(menu, Anchor::Center);
}

void LoadMacroPopup::refreshList()
{
    scroll->m_contentLayer->removeAllChildren();
    cells.clear();
    error->setString("");

    int CELL_HEIGHT = 40;

    auto count = as<int>(std::distance(std::filesystem::directory_iterator(path), std::filesystem::directory_iterator{}));

    m_listLayer->setOpacity(count == 0 ? 180 : 255);
    m_listLayer->setColor(count == 0 ? ccc3(0, 0, 0) : ccc3(25, 25, 25));

    if (count == 0)
    {
        error->setString("No macros saved <cl>:(</c>");
        return;
    }

    auto height = std::max<float>(count * CELL_HEIGHT, scroll->getContentHeight());
    scroll->m_contentLayer->setContentHeight(height);
    scroll->setTouchEnabled(height > scroll->getContentHeight());

    int i = 0;
    for (auto file : std::filesystem::directory_iterator(path))
    {
        auto cell = MacroCell::create(false, file.path().string(), ccp(scroll->getContentWidth(), CELL_HEIGHT), i, true);
        cell->setPositionY(height - (CELL_HEIGHT * i) - (CELL_HEIGHT / 2));
        cell->layerToDelete = this;
        scroll->m_contentLayer->addChild(cell);

        i++;
    }
}

void LoadMacroPopup::onRefresh(CCObject* sender)
{
    refreshList();
}

void LoadMacroPopup::onFolder(CCObject* sender)
{
    utils::file::openFolder(Mod::get()->getConfigDir() / "macros");
}

LoadMacroPopup* LoadMacroPopup::create()
{
    auto pRet = new LoadMacroPopup();

    if (pRet && pRet->init("Select Macro"))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}