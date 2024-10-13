#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CheckpointObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include "SillyBot.hpp"

using namespace geode::prelude;

void updateSettings()
{
	auto bot = SillyBot::get();

	bot->useFrames = Mod::get()->getSettingValue<bool>("frames");
	bot->clickFixes = Mod::get()->getSettingValue<bool>("click-fixes");
}

class $modify (PlayLayer)
{
	struct Fields {
		Ref<CCLabelBMFont> infoLabel;
	};

	void setupHasCompleted()
	{
		PlayLayer::setupHasCompleted();
		m_fields->infoLabel = getChildOfType<CCLabelBMFont>(this, 0);

		updateSettings();
	}
	
	void updateInfoLabel()
	{
		PlayLayer::updateInfoLabel();

		if (!Mod::get()->getSettingValue<bool>("info-additions"))
			return;

		if (m_fields->infoLabel)
		{
			std::stringstream ss;

			if (!Mod::get()->getSettingValue<bool>("info-replace"))
				ss << m_fields->infoLabel->getString();
			
			ss << "-- SillyBot --\n";
			ss << "Delta: ";
			ss << SillyBot::get()->deltaTime;
			ss << "\nFrame: ";
			ss << SillyBot::get()->curframe;
			ss << "\nPlayback Frame: ";
			ss << SillyBot::get()->frame;
			ss << "\nUse Frames: ";
			ss << (SillyBot::get()->useFrames ? "True" : "False");
			ss << "\nState: ";
			ss << SillyBot::get()->getStatusString();
			ss << "\nInput Count: ";
			ss << SillyBot::get()->macro.inputs.size();

			m_fields->infoLabel->setString(ss.str().c_str());
		}
	}
};

bool isUpdate;

class $modify (GJBaseGameLayer)
{
	void checkRepellPlayer()
	{
		isUpdate = true;
		GJBaseGameLayer::checkRepellPlayer();
	}

	void resetLevelVariables()
	{
		GJBaseGameLayer::resetLevelVariables();

		SillyBot::get()->deltaTime = 0;
		SillyBot::get()->frame = 0;
		SillyBot::get()->curframe = 0;
	}

	void updateCamera(float p0)
	{
		float dt = p0 / 60.0f;
		GJBaseGameLayer::updateCamera(p0);
		auto bot = SillyBot::get();

		if (isUpdate)
		{
			bot->deltaTime += dt;
			bot->updateCount++;
			bot->curframe++;

			if (bot->isPlaying() && bot->frame < bot->macro.inputs.size())
			{
				while ((bot->useFrames ? (bot->macro.inputs[bot->frame].frame < bot->curframe) : (bot->macro.inputs[bot->frame].dt < bot->deltaTime)) && (bot->frame < bot->macro.inputs.size()))
				{
					auto input = bot->macro.inputs[bot->frame];

					this->handleButton(input.down, input.button, !input.player2);

					if (bot->clickFixes)
						(input.player2 ? m_player2 : m_player1)->setPosition(input.pos);

					bot->frame++;
				}
			}

			isUpdate = false;
		}
	}

	void handleButton(bool down, int button, bool isPlayer1)
	{
		GJBaseGameLayer::handleButton(down, button, isPlayer1);
		auto bot = SillyBot::get();

		if (bot->isRecording())
		{
			bot->macro.inputs.push_back(SillyInput(bot->curframe, button, !isPlayer1, down, bot->deltaTime, (isPlayer1 ? m_player1 : m_player2)->getPosition()));
			bot->frame++;

			bot->macro.levelInfo.name = m_level->m_levelName;
			bot->macro.levelInfo.id = m_level->m_levelID;

			bot->macro.framerate = bot->getFrameRate();
		}
	}
};

class $modify (CheckpointObjectSave, CheckpointObject)
{
	struct Fields {
		float dt;
		int frame;
		int curframe;
	};

	virtual bool init()
	{
		if (!CheckpointObject::init())
			return false;

		m_fields->dt = SillyBot::get()->deltaTime;
		m_fields->frame = SillyBot::get()->frame;
		m_fields->curframe = SillyBot::get()->curframe;

		return true;
	}
};

class $modify (PlayLayer)
{
	struct Fields {
		bool willReloadCheckpoint;
		CheckpointObjectSave* cp;
	};

	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
	{
		SillyBot::get()->hasBottedRun = SillyBot::get()->getState() == State::Playing;

		return PlayLayer::init(level, useReplay, dontCreateObjects);
	}

	void resetLevel()
	{
		PlayLayer::resetLevel();

		auto bot = SillyBot::get();

		bot->hasBottedRun = bot->getState() == State::Playing;

		if (m_fields->cp)
		{
			bot->deltaTime = m_fields->cp->m_fields->dt;
			bot->frame = m_fields->cp->m_fields->frame;
			bot->curframe = m_fields->cp->m_fields->curframe;

			if (bot->isRecording())
			{
				while (bot->macro.inputs.size() > 0 && bot->useFrames ? (bot->macro.inputs[bot->macro.inputs.size() - 1].frame > bot->curframe) : (bot->macro.inputs[bot->macro.inputs.size() - 1].dt > bot->deltaTime))
				{
					bot->macro.inputs.pop_back();
				}
			}
		}
		else
		{
			if (SillyBot::get()->isRecording())
			{
				SillyBot::get()->macro.inputs.clear();
			}
		}

		if (SillyBot::get()->isRecording())
		{
			//if (SillyBot::get()->getLastFrame() != m_uiLayer->isJumpPressed())
			{
				
			}
		}

		m_fields->willReloadCheckpoint = false;
		m_fields->cp = nullptr;
	}

	void loadFromCheckpoint(CheckpointObject* p0)
	{
		PlayLayer::loadFromCheckpoint(p0);
		m_fields->willReloadCheckpoint = true;
		m_fields->cp = as<CheckpointObjectSave*>(p0);
	}
};

class $modify (CCKeyboardDispatcher)
{
	bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat)
	{
		if (isKeyDown && !isKeyRepeat && Mod::get()->getSettingValue<bool>("keybinds"))
		{
			if (key == enumKeyCodes::KEY_F1)
			{
				SillyBot::get()->setState(State::Disabled);
				auto alert = TextAlertPopup::create("Changed State To Disabled", 0.5f, 0.6f, 150, "");
				alert->setPositionY(50);
				CCScene::get()->addChild(alert, 9999999);
			}

			if (key == enumKeyCodes::KEY_F2)
			{
				SillyBot::get()->setState(State::Playing);
				auto alert = TextAlertPopup::create("Changed State To Playing", 0.5f, 0.6f, 150, "");
				alert->setPositionY(50);
				CCScene::get()->addChild(alert, 9999999);
			}

			if (key == enumKeyCodes::KEY_F3)
			{
				SillyBot::get()->setState(State::Recording);
				auto alert = TextAlertPopup::create("Changed State To Recording", 0.5f, 0.6f, 150, "");
				alert->setPositionY(50);
				CCScene::get()->addChild(alert, 9999999);
			}

			if (key == enumKeyCodes::KEY_F4)
			{
				SillyBot::get()->macro = SillyMacro();
				Notification::create("Cleared", NotificationIcon::Error, 0.1f)->show();
			}

			if (key == enumKeyCodes::KEY_F5)
			{
				SillyBot::get()->saveMacroToFile("C:\\Users\\talgo\\Desktop\\replay6.gdr.json", true);
				Notification::create("saved", NotificationIcon::Success, 0.1f)->show();
			}

			if (key == enumKeyCodes::KEY_F6)
			{
				SillyBot::get()->loadMacroFromFile("C:\\Users\\talgo\\Desktop\\Silent_Circles_15.gdr");
				Notification::create("loaded", NotificationIcon::Success, 0.1f)->show();
			}
		}

		return CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat);
	}
};