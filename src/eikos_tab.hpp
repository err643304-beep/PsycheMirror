#pragma once

#include "imgui.h"
#include "ics_tab.hpp"
#include <vector>
#include <string>

namespace Tabs {

struct ControlKnob {
    std::string name;
    float value;
    float min, max;
    std::vector<int> targetPixels;
    bool linked;
    
    ControlKnob() : value(0.5f), min(0.0f), max(1.0f), linked(false) {}
};

class EIKOSTab {
public:
    EIKOSTab();
    ~EIKOSTab() = default;

    void render();
    const char* getName() const { return "EIKOS"; }
    const char* getIcon() const { return ICON_EIKOS; }
    
    void setICSReference(ICSTab* ics) { icsRef = ics; }
    void renderKnobs();
    void renderNoiseControls();
    void renderPixelValueControls();
    void applyKnobToPixels(const ControlKnob& knob);

private:
    static constexpr const char* ICON_EIKOS = "EIKOS";
    
    ICSTab* icsRef;
    std::vector<ControlKnob> knobs;
    
    float noiseCancellation;
    float signalBoost;
    float contrastEnhancement;
    float brightnessAdjustment;
    bool autoNormalize;
    
    void addKnob();
    void removeKnob(int index);
    void renderKnobControl(ControlKnob& knob, int index);
};

}
