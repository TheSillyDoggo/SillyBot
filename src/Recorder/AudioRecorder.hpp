#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class AudioRecorder
{
    public:
        static inline bool isRecording = false;
        static inline FMOD::Sound* sound;

        static void startRecording();
        static void endRecording();
        static void toggleRecording();
};