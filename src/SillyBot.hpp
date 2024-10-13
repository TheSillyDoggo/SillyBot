#pragma once

#include <Geode/Geode.hpp>
#include "Bot.hpp"

using namespace geode::prelude;

enum State
{
    Disabled,
    Recording,
    Playing,
    Editing // built in replay editor ???
};

class SillyBot
{
    protected:
        static inline SillyBot* instance = nullptr;
        State replayState = State::Disabled;

    public:
        SillyMacro macro;
        float deltaTime;
        int frame;
        int curframe;
        int updateCount = 0;
        bool useFrames = false;
        bool clickFixes = true;
        bool hasBottedRun;


        static SillyBot* get();
        
        std::string getStatusString();
        CircleBaseColor colourFromStatus();

        State getState();
        void setState(State state);

        SillyMacro getMacro(std::string path, bool getInputs);
        void loadMacroFromFile(std::string path);
        void saveMacroToFile(std::string path, bool useJson);

        float getFrameRate();
        
        bool getLastFrame();

        bool isRecording();
        bool isPlaying();
};