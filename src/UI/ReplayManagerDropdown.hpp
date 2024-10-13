#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ReplayManagerDropdown : public GJDropDownLayer
{
    public:
        CCLayer* layer;
        TextArea* error;
        ScrollLayer* scroll;

        virtual void customSetup();

        void refreshList();

        static ReplayManagerDropdown* create();
};