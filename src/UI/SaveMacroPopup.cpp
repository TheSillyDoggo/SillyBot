#include "SaveMacroPopup.hpp"

bool SaveMacroPopup::setup()
{
    path = Mod::get()->getConfigDir() / "macros";

    if(!std::filesystem::exists(path))
        std::filesystem::create_directory(path);

    this->setAnchorPoint(ccp(0, 0));
    this->runAction(CCFadeTo::create(0.5f, 100));
    this->setKeypadEnabled(true);
    this->setTitle("Save Macro", "goldFont.fnt", 0.7f, 17.5f);

    m_mainLayer->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));
    m_mainLayer->updateLayout();

    input = TextInput::create(220, "Macro Name");
    input->setDelegate(this);
    input->setMaxCharCount(127);

    auto typeMenu = CCMenu::create();
    typeMenu->setScale(0.8f);
    typeMenu->setLayout(AxisLayout::create());

    gdr = CCMenuItemToggler::create(ButtonSprite::create(".GDR", 60, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_04.png", 30), ButtonSprite::create(".GDR", 60, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_01.png", 30), this, menu_selector(SaveMacroPopup::onGDR));
    gdrJson = CCMenuItemToggler::create(ButtonSprite::create(".GDR.JSON", 100, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_04.png", 30), ButtonSprite::create(".GDR.JSON", 100, 69420, 1.0f, true, "bigFont.fnt", "GJ_button_01.png", 30), this, menu_selector(SaveMacroPopup::onGDRJson));

    typeMenu->addChild(gdr);
    typeMenu->addChild(gdrJson);
    typeMenu->updateLayout();

    auto saveMenu = CCMenu::create();

    saveBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("Save"), this, menu_selector(SaveMacroPopup::onSave));
    as<ButtonSprite*>(saveBtn->getNormalImage())->setCascadeOpacityEnabled(true);
    as<ButtonSprite*>(saveBtn->getNormalImage())->setCascadeColorEnabled(true);
    saveBtn->getNormalImage()->setScale(0.8f);

    saveMenu->addChild(saveBtn);

    error = CCLabelBMFont::create("", "bigFont.fnt");
    error->setColor(ccc3(255, 70, 70));
    error->setOpacity(0);

    errorBG = CCScale9Sprite::create("square02_001.png");
    errorBG->setOpacity(0);
    errorBG->setScale(0.5f);

    updateSelectedType(false);

    m_mainLayer->addChildAtPosition(input, Anchor::Center, ccp(0, 40));
    m_mainLayer->addChildAtPosition(typeMenu, Anchor::Center, ccp(0, 0));
    m_mainLayer->addChildAtPosition(saveMenu, Anchor::Bottom, ccp(0, 21));
    this->addChildAtPosition(errorBG, Anchor::Center, ccp(0, -20 - m_size.height / 2));
    this->addChildAtPosition(error, Anchor::Center, ccp(0, -20 - m_size.height / 2));
    return true;
}

void SaveMacroPopup::updateInfo()
{
    bool canSave = true;
    error->runAction(CCFadeTo::create(0.5f, 0));
    errorBG->runAction(CCFadeTo::create(0.5f, 0));

    if (input->getString().size() == 0) // 127
        canSave = false;
    
    if (SillyBot::get()->macro.inputs.size() == 0)
    {
        canSave = false;
        error->setString("Empty Macro :(");
        error->limitLabelWidth(250, 0.6f, 0);
        errorBG->setContentSize(ccp(15 + error->getScaledContentWidth(), 25) * 2);

        error->runAction(CCFadeTo::create(0.5f, 255));
        errorBG->runAction(CCFadeTo::create(0.5f, 100));
    }

    if (std::filesystem::exists(fmt::format("{}/{}.{}", path, input->getString(), jsonSelected ? "gdr.json" : "gdr")))
    {
        canSave = false;
        error->setString("Macro already exists with same name");
        error->limitLabelWidth(250, 0.6f, 0);
        errorBG->setContentSize(ccp(15 + error->getScaledContentWidth(), 25) * 2);

        error->runAction(CCFadeTo::create(0.5f, 255));
        errorBG->runAction(CCFadeTo::create(0.5f, 100));
    }

    //as<ButtonSprite*>(saveBtn->getNormalImage())->setOpacity(canSave ? 255 : 100);
    as<ButtonSprite*>(saveBtn->getNormalImage())->setColor(canSave ? ccc3(255, 255, 255) : ccc3(150, 150, 150));
    saveBtn->setEnabled(canSave);
}

void SaveMacroPopup::updateSelectedType(bool json)
{
    gdr->setEnabled(json);
    gdrJson->setEnabled(!json);

    gdr->toggle(!json);
    gdrJson->toggle(json);

    jsonSelected = json;
    updateInfo();
}

void SaveMacroPopup::textChanged(CCTextInputNode* input)
{
    updateInfo();
}

void SaveMacroPopup::onGDR(CCObject*)
{
    updateSelectedType(false);
}

void SaveMacroPopup::onGDRJson(CCObject*)
{
    updateSelectedType(true);
}

void SaveMacroPopup::onSave(CCObject*)
{
    auto path = fmt::format("{}/{}.{}", this->path, input->getString(), jsonSelected ? "gdr.json" : "gdr");

    SillyBot::get()->saveMacroToFile(path, jsonSelected);

    if (!std::filesystem::exists(path))
    {
        FLAlertLayer::create("SillyBot", "<cr>Failed to save macro!</c>\nThis is probably because of an <cs>invalid name</c>.\nRemember you cannot use <cl>< > : \" / \\ | ? *</c>\nin file names on most <cp>operating systems</c>.", "OK")->show();
        return;
    }

    auto alert = TextAlertPopup::create("Saved Macro", 0.5f, 0.6f, 150, "");
    alert->setPositionY(50);
    CCScene::get()->addChild(alert, 9999999);

    this->onClose(nullptr);
}

SaveMacroPopup* SaveMacroPopup::create()
{
    auto pRet = new SaveMacroPopup();

    if (pRet && pRet->initAnchored(280, 180, "GJ_square02.png"))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}