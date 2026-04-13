#pragma once

#include "imgui.h"

namespace Tabs {

class HomeTab {
public:
    HomeTab() = default;
    ~HomeTab() = default;

    void render();
    const char* getName() const { return "Home"; }
    const char* getIcon() const { return ICON_HOME; }

private:
    static constexpr const char* ICON_HOME = 
        "M10 20v-6h4v6h5v-8h3L12 3 2 12h3v8z";
};

}
