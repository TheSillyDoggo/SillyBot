#include "ReplayManagerDropdown.hpp"

void ReplayManagerDropdown::customSetup()
{
    layer = CCLayer::create();
    layer->ignoreAnchorPointForPosition(false);
    layer->setPosition(CCDirector::get()->getWinSize() / 2);
    m_mainLayer->addChild(layer);

    scroll = ScrollLayer::create(m_listLayer->getContentSize());
    scroll->setZOrder(-1);

    error = TextArea::create("", "bigFont.fnt", 1, 1000, ccp(0.5f, 0.5f), 10, false);
    error->setScale(0.65f);

    refreshList();

    m_listLayer->addChild(scroll);
    layer->addChildAtPosition(error, Anchor::Center);
}

void ReplayManagerDropdown::refreshList()
{
    scroll->m_contentLayer->removeAllChildren();
    error->setString("");

    if (!std::filesystem::exists(Mod::get()->getConfigDir() / "macros/"))
        return error->setString("No macros saved <cl>:(</c>");

    for (auto file : Mod::get()->getConfigDir() / "macros/")
    {
        
    }
}

ReplayManagerDropdown* ReplayManagerDropdown::create()
{
    auto pRet = new ReplayManagerDropdown();

    if (pRet && pRet->init("Macros"))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}