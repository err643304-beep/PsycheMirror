#include "csi_tab.hpp"
#include "data_bridge.hpp"
#include <cmath>
#include <cstring>

namespace Tabs {

CSITab::CSITab() 
    : icsRef(nullptr), selectedGroup(-1), showECI(false), showACI(false) {
    memset(equationBuffer, 0, sizeof(equationBuffer));
    
    ACIAlgorithm wave;
    wave.name = "Wave Pattern";
    wave.code = "sin(t + x)";
    wave.parameters = {1.0f, 0.5f};
    aciAlgorithms.push_back(wave);
    
    ACIAlgorithm spiral;
    spiral.name = "Spiral";
    spiral.code = "cos(sqrt(x*x + y*y) - t)";
    spiral.parameters = {2.0f, 1.0f};
    aciAlgorithms.push_back(spiral);
}

void CSITab::render() {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.04f, 0.04f, 0.04f, 1.0f));
    
    ImGui::BeginChild("CSIMain", ImVec2(showACI ? -250 : 0, 0), true);
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("CSI - Control Sub Pixels");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    if (ImGui::Button("Toggle ECI Monitor")) {
        showECI = !showECI;
    }
    ImGui::SameLine();
    if (ImGui::Button("Toggle ACI Sidebar")) {
        showACI = !showACI;
    }
    ImGui::SameLine();
    if (ImGui::Button("Add Pixel Group")) {
        addNewPixelGroup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Broadcast Equation")) {
        if (!pixelGroups.empty()) {
            DataBridge::getInstance().broadcastEquation(pixelGroups[0].equation);
        }
    }
    
    ImGui::Separator();
    
    if (showECI) {
        ImGui::BeginChild("ECISection", ImVec2(0, 300), true);
        renderECIMonitor();
        ImGui::EndChild();
    }
    
    renderControlPanel();
    
    ImGui::EndChild();
    
    if (showACI) {
        ImGui::SameLine();
        ImGui::BeginChild("ACISidebar", ImVec2(0, 0), true);
        renderACISidebar();
        ImGui::EndChild();
    }
    
    ImGui::PopStyleColor();
}

void CSITab::renderControlPanel() {
    ImGui::BeginChild("ControlPanel", ImVec2(0, 0), true);
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("Pixel Group Control");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    for (size_t i = 0; i < pixelGroups.size(); i++) {
        ImGui::PushID(i);
        
        if (ImGui::CollapsingHeader(("Group " + std::to_string(i)).c_str())) {
            PixelGroup& group = pixelGroups[i];
            
            ImGui::Checkbox("Active", &group.active);
            
            ImGui::SliderFloat("Contrast", &group.contrast, 0.0f, 2.0f);
            ImGui::SliderFloat("Brightness", &group.brightness, -1.0f, 1.0f);
            
            ImGui::Text("Pixels: %zu", group.pixelIndices.size());
            
            char buf[512];
            strncpy(buf, group.equation.c_str(), sizeof(buf));
            if (ImGui::InputTextMultiline("Equation", buf, sizeof(buf), ImVec2(-1, 60))) {
                group.equation = buf;
            }
            
            if (ImGui::Button("Apply Equation")) {
                applyEquationToGroup(group);
            }
            ImGui::SameLine();
            if (ImGui::Button("Remove Group")) {
                removePixelGroup(i);
            }
            
            if (ImGui::Button("Import from ICS Selection")) {
                if (icsRef) {
                    group.pixelIndices.clear();
                    auto& pixels = icsRef->getPixels();
                    for (size_t j = 0; j < pixels.size(); j++) {
                        if (pixels[j].selected) {
                            group.pixelIndices.push_back(j);
                        }
                    }
                }
            }
        }
        
        ImGui::PopID();
    }
    
    ImGui::EndChild();
}

void CSITab::renderECIMonitor() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("ECI - Multi-Pixel Monitor & Comparison");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    ImGui::Checkbox("Show Comparison", &eciMonitor.showComparison);
    ImGui::SameLine();
    ImGui::Checkbox("Sync Updates", &eciMonitor.syncUpdates);
    
    if (ImGui::Button("Import to Monitor 1")) {
        if (icsRef) {
            eciMonitor.monitor1Pixels.clear();
            auto& pixels = icsRef->getPixels();
            for (size_t i = 0; i < pixels.size(); i++) {
                if (pixels[i].selected) {
                    eciMonitor.monitor1Pixels.push_back(i);
                }
            }
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Import to Monitor 2")) {
        if (icsRef) {
            eciMonitor.monitor2Pixels.clear();
            auto& pixels = icsRef->getPixels();
            for (size_t i = 0; i < pixels.size(); i++) {
                if (pixels[i].selected) {
                    eciMonitor.monitor2Pixels.push_back(i);
                }
            }
        }
    }
    
    ImGui::Columns(2, "MonitorColumns");
    
    ImGui::Text("Monitor 1 (%zu pixels)", eciMonitor.monitor1Pixels.size());
    ImGui::BeginChild("Monitor1", ImVec2(0, 150), true);
    if (icsRef) {
        auto& pixels = icsRef->getPixels();
        for (int idx : eciMonitor.monitor1Pixels) {
            if (idx < pixels.size()) {
                ImGui::ColorButton(("##mon1_" + std::to_string(idx)).c_str(), 
                    ImVec4(pixels[idx].r, pixels[idx].g, pixels[idx].b, pixels[idx].a),
                    0, ImVec2(20, 20));
                if ((idx + 1) % 10 != 0) ImGui::SameLine();
            }
        }
    }
    ImGui::EndChild();
    
    ImGui::NextColumn();
    
    ImGui::Text("Monitor 2 (%zu pixels)", eciMonitor.monitor2Pixels.size());
    ImGui::BeginChild("Monitor2", ImVec2(0, 150), true);
    if (icsRef) {
        auto& pixels = icsRef->getPixels();
        for (int idx : eciMonitor.monitor2Pixels) {
            if (idx < pixels.size()) {
                ImGui::ColorButton(("##mon2_" + std::to_string(idx)).c_str(), 
                    ImVec4(pixels[idx].r, pixels[idx].g, pixels[idx].b, pixels[idx].a),
                    0, ImVec2(20, 20));
                if ((idx + 1) % 10 != 0) ImGui::SameLine();
            }
        }
    }
    ImGui::EndChild();
    
    ImGui::Columns(1);
}

void CSITab::renderACISidebar() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("ACI - Algorithm Control");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    for (size_t i = 0; i < aciAlgorithms.size(); i++) {
        ImGui::PushID(i);
        
        ACIAlgorithm& algo = aciAlgorithms[i];
        
        if (ImGui::CollapsingHeader(algo.name.c_str())) {
            ImGui::Checkbox("Enabled", &algo.enabled);
            
            ImGui::TextWrapped("Code: %s", algo.code.c_str());
            
            for (size_t p = 0; p < algo.parameters.size(); p++) {
                std::string label = "Param " + std::to_string(p);
                ImGui::SliderFloat(label.c_str(), &algo.parameters[p], 0.0f, 10.0f);
            }
            
            if (ImGui::Button("Apply to Selected Group")) {
                if (selectedGroup >= 0 && selectedGroup < pixelGroups.size()) {
                    pixelGroups[selectedGroup].equation = algo.code;
                }
            }
        }
        
        ImGui::PopID();
    }
}

void CSITab::applyEquationToGroup(PixelGroup& group) {
    if (!icsRef) return;
    
    std::lock_guard<std::mutex> lock(icsRef->getPixelMutex());
    auto& pixels = icsRef->getPixels();
    
    for (int idx : group.pixelIndices) {
        if (idx < pixels.size()) {
            pixels[idx].equation = group.equation;
        }
    }
}

void CSITab::addNewPixelGroup() {
    PixelGroup group;
    pixelGroups.push_back(group);
}

void CSITab::removePixelGroup(int index) {
    if (index >= 0 && index < pixelGroups.size()) {
        pixelGroups.erase(pixelGroups.begin() + index);
    }
}

}
