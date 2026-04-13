#pragma once

#include "imgui.h"
#include "ics_tab.hpp"
#include <vector>
#include <string>
#include <memory>

namespace Tabs {

struct PixelGroup {
    std::vector<int> pixelIndices;
    std::string equation;
    float contrast;
    float brightness;
    bool active;
    
    PixelGroup() : contrast(1.0f), brightness(0.0f), active(true) {}
};

struct ECIMonitor {
    std::vector<int> monitor1Pixels;
    std::vector<int> monitor2Pixels;
    bool showComparison;
    bool syncUpdates;
    
    ECIMonitor() : showComparison(false), syncUpdates(true) {}
};

struct ACIAlgorithm {
    std::string name;
    std::string code;
    std::vector<float> parameters;
    bool enabled;
    
    ACIAlgorithm() : enabled(false) {}
};

class CSITab {
public:
    CSITab();
    ~CSITab() = default;

    void render();
    const char* getName() const { return "CSI"; }
    const char* getIcon() const { return ICON_CSI; }
    
    void setICSReference(ICSTab* ics) { icsRef = ics; }
    void renderControlPanel();
    void renderECIMonitor();
    void renderACISidebar();
    
private:
    static constexpr const char* ICON_CSI = "CSI";
    
    ICSTab* icsRef;
    std::vector<PixelGroup> pixelGroups;
    ECIMonitor eciMonitor;
    std::vector<ACIAlgorithm> aciAlgorithms;
    
    int selectedGroup;
    bool showECI;
    bool showACI;
    char equationBuffer[512];
    
    void applyEquationToGroup(PixelGroup& group);
    void addNewPixelGroup();
    void removePixelGroup(int index);
};

}
