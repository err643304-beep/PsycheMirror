#include "eikos_tab.hpp"
#include "data_bridge.hpp"
#include <algorithm>
#include <cmath>

namespace Tabs {

EIKOSTab::EIKOSTab() 
    : icsRef(nullptr), noiseCancellation(0.0f), signalBoost(1.0f), 
      contrastEnhancement(1.0f), brightnessAdjustment(0.0f), autoNormalize(false) {
    
    ControlKnob redKnob;
    redKnob.name = "Red Channel";
    redKnob.value = 0.5f;
    knobs.push_back(redKnob);
    
    ControlKnob greenKnob;
    greenKnob.name = "Green Channel";
    greenKnob.value = 0.5f;
    knobs.push_back(greenKnob);
    
    ControlKnob blueKnob;
    blueKnob.name = "Blue Channel";
    blueKnob.value = 0.5f;
    knobs.push_back(blueKnob);
    
    ControlKnob masterKnob;
    masterKnob.name = "Master";
    masterKnob.value = 1.0f;
    knobs.push_back(masterKnob);
}

void EIKOSTab::render() {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.04f, 0.04f, 0.04f, 1.0f));
    ImGui::BeginChild("EIKOSContent", ImVec2(0, 0), true);
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("EIKOS - Pixel Control & Noise Cancellation");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    if (ImGui::Button("Add Knob")) {
        addKnob();
    }
    ImGui::SameLine();
    if (ImGui::Button("Link Selected Pixels")) {
        if (icsRef && !knobs.empty()) {
            std::lock_guard<std::mutex> lock(icsRef->getPixelMutex());
            auto& pixels = icsRef->getPixels();
            for (size_t i = 0; i < pixels.size(); i++) {
                if (pixels[i].selected && 
                    std::find(knobs[0].targetPixels.begin(), 
                             knobs[0].targetPixels.end(), i) == knobs[0].targetPixels.end()) {
                    knobs[0].targetPixels.push_back(i);
                }
            }
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Link to Arduino")) {
        for (size_t i = 0; i < knobs.size() && i < 10; i++) {
            DataBridge::getInstance().setArduinoPinValue(i + 2, knobs[i].value);
        }
    }
    
    ImGui::Separator();
    
    ImGui::Columns(2, "EIKOSColumns");
    
    ImGui::BeginChild("KnobPanel", ImVec2(0, 0), true);
    renderKnobs();
    ImGui::EndChild();
    
    ImGui::NextColumn();
    
    ImGui::BeginChild("ControlPanel", ImVec2(0, 0), true);
    renderNoiseControls();
    ImGui::Separator();
    renderPixelValueControls();
    ImGui::EndChild();
    
    ImGui::Columns(1);
    
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void EIKOSTab::renderKnobs() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("Control Knobs");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    for (size_t i = 0; i < knobs.size(); i++) {
        renderKnobControl(knobs[i], i);
    }
}

void EIKOSTab::renderKnobControl(ControlKnob& knob, int index) {
    ImGui::PushID(index);
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("%s", knob.name.c_str());
    ImGui::PopStyleColor();
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 center = ImGui::GetCursorScreenPos();
    center.x += 60;
    center.y += 60;
    
    float radius = 50.0f;
    float angle = (knob.value - knob.min) / (knob.max - knob.min) * 3.14159f * 1.5f - 3.14159f * 0.75f;
    
    draw_list->AddCircle(center, radius, IM_COL32(0, 255, 136, 255), 32, 2.0f);
    
    for (int i = 0; i <= 10; i++) {
        float tick_angle = -3.14159f * 0.75f + i * (3.14159f * 1.5f / 10.0f);
        float tick_start = radius - 5;
        float tick_end = radius;
        ImVec2 start(center.x + cos(tick_angle) * tick_start, center.y + sin(tick_angle) * tick_start);
        ImVec2 end(center.x + cos(tick_angle) * tick_end, center.y + sin(tick_angle) * tick_end);
        draw_list->AddLine(start, end, IM_COL32(100, 255, 136, 200), 1.0f);
    }
    
    ImVec2 pointer_end(center.x + cos(angle) * (radius - 10), center.y + sin(angle) * (radius - 10));
    draw_list->AddLine(center, pointer_end, IM_COL32(255, 136, 0, 255), 3.0f);
    draw_list->AddCircleFilled(center, 8.0f, IM_COL32(0, 255, 136, 255));
    
    ImGui::Dummy(ImVec2(120, 120));
    
    if (ImGui::IsItemHovered() && ImGui::IsMouseDragging(0)) {
        ImVec2 mouse = ImGui::GetMousePos();
        float dx = mouse.x - center.x;
        float dy = mouse.y - center.y;
        float mouse_angle = atan2(dy, dx);
        
        mouse_angle = fmod(mouse_angle + 3.14159f * 2.75f, 3.14159f * 2.0f);
        if (mouse_angle > 3.14159f * 1.5f) mouse_angle = 3.14159f * 1.5f;
        
        knob.value = knob.min + (mouse_angle / (3.14159f * 1.5f)) * (knob.max - knob.min);
        applyKnobToPixels(knob);
    }
    
    if (ImGui::SliderFloat(("##slider" + std::to_string(index)).c_str(), &knob.value, knob.min, knob.max)) {
        applyKnobToPixels(knob);
    }
    
    ImGui::Text("Linked Pixels: %zu", knob.targetPixels.size());
    
    ImGui::Checkbox("Linked", &knob.linked);
    
    if (ImGui::Button("Remove")) {
        removeKnob(index);
    }
    
    ImGui::Separator();
    ImGui::PopID();
}

void EIKOSTab::renderNoiseControls() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("Noise Cancellation");
    ImGui::PopStyleColor();
    
    ImGui::SliderFloat("Noise Reduction", &noiseCancellation, 0.0f, 1.0f);
    ImGui::SliderFloat("Signal Boost", &signalBoost, 0.0f, 2.0f);
    
    if (ImGui::Button("Apply Noise Filter")) {
        if (icsRef) {
            std::lock_guard<std::mutex> lock(icsRef->getPixelMutex());
            auto& pixels = icsRef->getPixels();
            for (auto& p : pixels) {
                if (p.selected) {
                    p.r = std::clamp(p.r * signalBoost - noiseCancellation, 0.0f, 1.0f);
                    p.g = std::clamp(p.g * signalBoost - noiseCancellation, 0.0f, 1.0f);
                    p.b = std::clamp(p.b * signalBoost - noiseCancellation, 0.0f, 1.0f);
                }
            }
        }
    }
}

void EIKOSTab::renderPixelValueControls() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("Pixel Value Adjustment");
    ImGui::PopStyleColor();
    
    ImGui::SliderFloat("Contrast", &contrastEnhancement, 0.0f, 2.0f);
    ImGui::SliderFloat("Brightness", &brightnessAdjustment, -1.0f, 1.0f);
    ImGui::Checkbox("Auto Normalize", &autoNormalize);
    
    if (ImGui::Button("Apply Adjustments")) {
        if (icsRef) {
            std::lock_guard<std::mutex> lock(icsRef->getPixelMutex());
            auto& pixels = icsRef->getPixels();
            for (auto& p : pixels) {
                if (p.selected) {
                    p.r = std::clamp((p.r - 0.5f) * contrastEnhancement + 0.5f + brightnessAdjustment, 0.0f, 1.0f);
                    p.g = std::clamp((p.g - 0.5f) * contrastEnhancement + 0.5f + brightnessAdjustment, 0.0f, 1.0f);
                    p.b = std::clamp((p.b - 0.5f) * contrastEnhancement + 0.5f + brightnessAdjustment, 0.0f, 1.0f);
                }
            }
        }
    }
    
    if (ImGui::Button("Reset to Default")) {
        contrastEnhancement = 1.0f;
        brightnessAdjustment = 0.0f;
        noiseCancellation = 0.0f;
        signalBoost = 1.0f;
    }
}

void EIKOSTab::applyKnobToPixels(const ControlKnob& knob) {
    if (!icsRef || !knob.linked) return;
    
    std::lock_guard<std::mutex> lock(icsRef->getPixelMutex());
    auto& pixels = icsRef->getPixels();
    
    for (int idx : knob.targetPixels) {
        if (idx >= 0 && idx < pixels.size()) {
            if (knob.name.find("Red") != std::string::npos) {
                pixels[idx].r = knob.value;
            } else if (knob.name.find("Green") != std::string::npos) {
                pixels[idx].g = knob.value;
            } else if (knob.name.find("Blue") != std::string::npos) {
                pixels[idx].b = knob.value;
            } else if (knob.name.find("Master") != std::string::npos) {
                pixels[idx].r *= knob.value;
                pixels[idx].g *= knob.value;
                pixels[idx].b *= knob.value;
            }
        }
    }
}

void EIKOSTab::addKnob() {
    ControlKnob newKnob;
    newKnob.name = "Knob " + std::to_string(knobs.size() + 1);
    knobs.push_back(newKnob);
}

void EIKOSTab::removeKnob(int index) {
    if (index >= 0 && index < knobs.size()) {
        knobs.erase(knobs.begin() + index);
    }
}

}
