#include "SillyBot.hpp"
#include "Hooks/PauseLayer.hpp"
#include "SillyPopup.hpp"

SillyBot* SillyBot::get()
{
    if (!instance)
    {
        instance = new SillyBot();
    }

    return instance;
}

SillyMacro SillyBot::getMacro(std::string path, bool getInputs)
{
    bool useJson = path.ends_with(".json");
    useJson = false;
    SillyMacro macro;

    if (useJson)
    {
        std::ifstream f(path, std::ios::binary);

        f.seekg(0, std::ios::end);
        size_t fileSize = f.tellg();
        f.seekg(0, std::ios::beg);

        std::vector<std::uint8_t> macroData(fileSize);

        f.read(reinterpret_cast<char *>(macroData.data()), fileSize);
        f.close();

        macro = SillyMacro::importData(macroData, getInputs);
        macro = SillyMacro::importData(macroData, getInputs);
    }
    else
    {        
        std::ifstream f(path, std::ios::binary);

        f.seekg(0, std::ios::end);
        size_t fileSize = f.tellg();
        f.seekg(0, std::ios::beg);

        std::vector<std::uint8_t> macroData(fileSize);

        f.read(reinterpret_cast<char *>(macroData.data()), fileSize);
        f.close();

        //yep.

        macro = SillyMacro::importData(macroData, getInputs);
        macro = SillyMacro::importData(macroData, getInputs);
    }

    return macro;
}

void SillyBot::loadMacroFromFile(std::string path)
{
    macro = getMacro(path, true);
}

void SillyBot::saveMacroToFile(std::string path, bool useJson)
{
    if (useJson)
    {
        std::ofstream f(path, std::ios::binary);
        auto data = macro.exportData(true);

        f.write(reinterpret_cast<const char *>(data.data()), data.size());
        f.close();
    }
    else
    {
        std::ofstream f(path, std::ios::binary);
        auto data = macro.exportData(false);

        f.write(reinterpret_cast<const char *>(data.data()), data.size());
        f.close();
    }
}

std::string SillyBot::getStatusString()
{
    if (replayState == State::Disabled)
        return "Disabled";
    else if (replayState == State::Playing)
        return "Playing";
    else if (replayState == State::Recording)
        return "Recording";
    else
        return "Editing";
}

CircleBaseColor SillyBot::colourFromStatus()
{
    if (replayState == State::Disabled)
        return CircleBaseColor::Green;
    else if (replayState == State::Playing)
        return CircleBaseColor::Pink;
    else if (replayState == State::Recording)
        return CircleBaseColor::Cyan;
    else
        return CircleBaseColor::Gray;
}

float SillyBot::getFrameRate()
{
    return 1 / 240;

    #ifdef GEODE_IS_WINDOWS
    int offset = geode::base::get() + 0x49D548;
    #endif

    #ifdef GEODE_IS_ANDROID32
    int offset = geode::base::get() + (0x467EB8 - 0x10000);
    #endif

    #ifdef GEODE_IS_ANDROID64
    int offset = geode::base::get() + (0x9335c0 - 0x100000);
    #endif

    #ifdef GEODE_IS_MACOS
    int offset = geode::base::get() + 0x7e9c60;
    #endif

    return 1.0f / (*(float*)((char*)offset));
}

bool SillyBot::isRecording()
{
    return replayState == State::Recording;
}

bool SillyBot::isPlaying()
{
    return replayState == State::Playing;
}

bool SillyBot::getLastFrame()
{
    if (macro.inputs.size() > 0)
    {
        return macro.inputs[macro.inputs.size() - 1].down;
    }

    return false;
}

State SillyBot::getState()
{
    return replayState;
}

void SillyBot::setState(State state)
{
    replayState = state;
    
    if (state == State::Playing)
        hasBottedRun = true;

    if (auto pause = PauseLayerExt::get())
        pause->updateButtonState();

    if (auto popup = SillyPopup::get())
        popup->onStateChanged();
}