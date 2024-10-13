/*#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class VideoRecorder : public CCNode
{
    public:
        PlayLayer* gameLayer;
        std::vector<CCRenderTexture*> textures;

        bool initWithPlayLayer(GJGameLevel* level);
        static VideoRecorder* createWithPlayLayer(GJGameLevel* level);

        void renderFrames(int frames, float frameRate);
};*/