#pragma once

#include "imgui.h"

namespace Tabs {

class ViewerTab {
public:
    ViewerTab() = default;
    ~ViewerTab() = default;

    void render();
    const char* getName() const { return "Viewer"; }
    const char* getIcon() const { return ICON_VIEWER; }

private:
    float rotation = 0.0f;
    static constexpr const char* ICON_VIEWER = 
        "M12 4.5C7 4.5 2.73 7.61 1 12c1.73 4.39 6 7.5 11 7.5s9.27-3.11 11-7.5c-1.73-4.39-6-7.5-11-7.5zM12 17c-2.76 0-5-2.24-5-5s2.24-5 5-5 5 2.24 5 5-2.24 5-5 5zm0-8c-1.66 0-3 1.34-3 3s1.34 3 3 3 3-1.34 3-3-1.34-3-3-3z";
};

}
