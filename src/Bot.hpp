#pragma once

#include <gdr/gdr.hpp>
#include <Geode/Geode.hpp>

struct SillyInput : gdr::Input {
    SillyInput() = default;
    float dt;
    cocos2d::CCPoint pos;

    SillyInput(int frame, int button, bool player2, bool down, float dt, cocos2d::CCPoint pos)
        : Input(frame, button, player2, down), dt(dt), pos(pos) {}

    void parseExtension(gdr::json::object_t obj) override {
        if (obj.count("dt") > 0)
		    dt = obj["dt"];
        
        if (obj.count("posx") > 0)
            pos.x = obj["posx"];

        if (obj.count("posy") > 0)
            pos.y = obj["posy"];

        if (obj.count("mhr_x") > 0)
            pos.x = obj["mhr_x"];

        if (obj.count("mhr_y") > 0)
            pos.y = obj["mhr_y"];

        if (obj.count("frame") > 0)
            frame = obj["frame"].get<int>();
	}

	gdr::json::object_t saveExtension() const override {
		return { {"dt", dt}, {"posx", pos.x}, {"posy", pos.y} };
	}
};

struct SillyMacro : gdr::Replay<SillyMacro, SillyInput> {
    SillyMacro() : Replay("SillyBot", geode::Mod::get()->getVersion().toString().c_str()) {}
};