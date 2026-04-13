#pragma once

#include "imgui.h"
#include "ics_tab.hpp"
#include <vector>
#include <string>

namespace Tabs {

struct LargeSubPixel {
    int pixelIndex;
    int row, col;
    float displaySize;
    bool showLabel;
    
    LargeSubPixel() : pixelIndex(-1), row(0), col(0), displaySize(50.0f), showLabel(true) {}
};

class LSITab {
public:
    LSITab();
    ~LSITab() = default;

    void render();
    const char* getName() const { return "LSI"; }
    const char* getIcon() const { return ICON_LSI; }
    
    void setICSReference(ICSTab* ics) { icsRef = ics; }
    void renderLargePixelView();
    void renderContextMenu();
    void handleRightClick();
    void exportToIKS();

private:
    static constexpr const char* ICON_LSI = "LSI";
    
    ICSTab* icsRef;
    std::vector<LargeSubPixel> largePixels;
    
    int hoveredPixelIndex;
    int contextMenuPixelIndex;
    bool showContextMenu;
    float globalDisplaySize;
    bool showLabels;
    bool showValues;
    
    void syncWithICS();
    void addPixelToView(int pixelIndex);
    void removePixelFromView(int index);
};

}
