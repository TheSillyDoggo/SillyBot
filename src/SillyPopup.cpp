#include "SillyPopup.hpp"
#include "UI/LoadMacroPopup.hpp"
#include "UI/SaveMacroPopup.hpp"

Ref<SillyPopup> ins;
#define HorizontalInset 8
#define VerticalInset 9

bool SillyPopup::setup()
{
    ins = this;

    this->setAnchorPoint(ccp(0, 0));
    this->runAction(CCFadeTo::create(0.5f, 100));
    this->setKeypadEnabled(true);
    //this->setTitle("SillyBot", "goldFont.fnt", 0.7f, 16.5f);

    m_mainLayer->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));
    m_mainLayer->updateLayout();

    auto bgRight = CCScale9Sprite::create("square02_001.png");
    bgRight->setAnchorPoint(ccp(1, 0.5f));
    bgRight->setOpacity(100);
    bgRight->setContentWidth((this->m_size.width / 2) - HorizontalInset * 2);
    bgRight->setContentHeight(this->m_size.height - VerticalInset * 2);
    bgRight->setScale(1.0f / 2.0f);
    bgRight->setContentSize(bgRight->getContentSize() * 2);

    setupSettings(bgRight);

    auto rightMenu = CCMenu::create();
    rightMenu->setLayout(AxisLayout::create()->setAxis(Axis::Column));
    rightMenu->setAnchorPoint(ccp(1, 0.5f));

    auto leftMenu = CCMenu::create();
    leftMenu->setLayout(AxisLayout::create()->setAxis(Axis::Column));
    leftMenu->setAnchorPoint(ccp(0, 0.5f));

    auto save = CCMenuItemSpriteExtra::create(ButtonSprite::create("Save Macro", 100, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_01.png", 30), this, menu_selector(SillyPopup::onSave));
    auto load = CCMenuItemSpriteExtra::create(ButtonSprite::create("Load Macro", 100, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_01.png", 30), this, menu_selector(SillyPopup::onLoad));
    auto options = CCMenuItemSpriteExtra::create(ButtonSprite::create("Options", 100, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_01.png", 30), this, menu_selector(SillyPopup::onSettings));

    disable = CCMenuItemToggler::create(ButtonSprite::create("Disabled", 100, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_04.png", 30), ButtonSprite::create("Disabled", 100, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_01.png", 30), this, menu_selector(SillyPopup::onChangeState));
    as<ButtonSprite*>(disable->m_offButton->getNormalImage())->setCascadeOpacityEnabled(true);
    as<ButtonSprite*>(disable->m_offButton->getNormalImage())->setOpacity(150);
    disable->setTag(State::Disabled);

    record = CCMenuItemToggler::create(ButtonSprite::create("Record", 100, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_04.png", 30), ButtonSprite::create("Record", 100, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_01.png", 30), this, menu_selector(SillyPopup::onChangeState));
    as<ButtonSprite*>(record->m_offButton->getNormalImage())->setCascadeOpacityEnabled(true);
    as<ButtonSprite*>(record->m_offButton->getNormalImage())->setOpacity(150);
    record->setTag(State::Recording);

    playback = CCMenuItemToggler::create(ButtonSprite::create("Playback", 100, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_04.png", 30), ButtonSprite::create("Playback", 100, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_01.png", 30), this, menu_selector(SillyPopup::onChangeState));
    as<ButtonSprite*>(playback->m_offButton->getNormalImage())->setCascadeOpacityEnabled(true);
    as<ButtonSprite*>(playback->m_offButton->getNormalImage())->setOpacity(150);
    playback->setTag(State::Playing);

    rightMenu->addChild(load);
    rightMenu->addChild(save);
    rightMenu->addChild(options);
    rightMenu->updateLayout();

    leftMenu->addChild(record);
    leftMenu->addChild(playback);
    leftMenu->addChild(disable);
    leftMenu->updateLayout();
    onStateChanged();

    m_mainLayer->addChildAtPosition(bgRight, Anchor::Right, ccp(-HorizontalInset, 0));
    m_mainLayer->addChildAtPosition(rightMenu, Anchor::Right, ccp(-10, 0));
    m_mainLayer->addChildAtPosition(leftMenu, Anchor::Left, ccp(10, 0));
    return true;
}

void SillyPopup::setupSettings(CCScale9Sprite* sprite)
{
    auto menu = CCMenu::create();
    menu->setScale(2);
    menu->setContentSize(sprite->getContentSize() / 2);
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(ccp(0, 0));

    auto title = CCLabelBMFont::create("Settings", "goldFont.fnt");
    title->setScale(0.7f);

    int i = 0;
    for (auto key : Mod::get()->getSettingKeys())
    {
        auto setting = as<BoolSettingValue*>(Mod::get()->getSetting(key));

        auto name = CCLabelBMFont::create(setting->getDefinition().getDisplayName().c_str(), "bigFont.fnt");
        name->setAnchorPoint(ccp(0, 0.5f));
        name->limitLabelWidth(menu->getContentWidth() / 3 * 2, 0.55f, 0);

        menu->addChildAtPosition(name, Anchor::TopLeft, ccp(4, -35 - (20 * i)));
        i++;
    }

    menu->addChildAtPosition(title, Anchor::Top, ccp(0, -12));
    sprite->addChild(menu);
}

void SillyPopup::onClose(CCObject* sender)
{
    this->removeFromParent();
}

void SillyPopup::onChangeState(CCObject* sender)
{
    SillyBot::get()->setState(as<State>(sender->getTag()));
}

void SillyPopup::onLoad(CCObject* sender)
{
    auto popup = LoadMacroPopup::create();
    popup->showLayer(false);
    popup->setZOrder(this->getZOrder() + 1);
}

void SillyPopup::onSave(CCObject* sender)
{
    auto popup = SaveMacroPopup::create();
    popup->show();
}

void SillyPopup::keyBackClicked()
{
    onClose(nullptr);
}

void SillyPopup::onStateChanged()
{
    disable->toggle(SillyBot::get()->getState() == State::Disabled);
    record->toggle(SillyBot::get()->getState() == State::Recording);
    playback->toggle(SillyBot::get()->getState() == State::Playing);

    disable->setEnabled(SillyBot::get()->getState() != State::Disabled);
    record->setEnabled(SillyBot::get()->getState() != State::Recording);
    playback->setEnabled(SillyBot::get()->getState() != State::Playing);
}

void SillyPopup::updateMacroInfo()
{
    std::stringstream ss;
    auto macro = SillyBot::get()->macro;

    if (!macro.inputs.size())
    {
        ss << "No Macro Loaded";
    }
    else
    {
        ss << "Level Name: ";
        ss << macro.levelInfo.name;
        ss << "\nLevel ID: ";
        ss << macro.levelInfo.id;
        ss << "\nSeed: ";
        ss << macro.seed;
        ss << "\nFramerate: ";
        ss << macro.framerate;

        ss << "\nBot Version: ";
        ss << macro.botInfo.version;
    }

    macroInfo->setString(ss.str().c_str());
}

void SillyPopup::onSettings(CCObject*)
{
    Mod::get()->setSettingValue<bool>("", false);

    geode::openSettingsPopup(Mod::get());
}

SillyPopup* SillyPopup::create()
{
    auto pRet = new SillyPopup();

    if (pRet && pRet->initAnchored(380, 240, "GJ_square02.png"))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

SillyPopup* SillyPopup::get()
{
    return ins;
}