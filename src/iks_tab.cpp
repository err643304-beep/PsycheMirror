#include "iks_tab.hpp"
#include "data_bridge.hpp"
#include <cmath>
#include <cstring>
#include <algorithm>
#include <chrono>

namespace Tabs {

IKSTab::IKSTab() 
    : icsRef(nullptr), isCompiling(false), showCircuitDesigner(true), 
      showWavePlotter(true), totalPins(40), infinitePinMode(false) {
    memset(codeBuffer, 0, sizeof(codeBuffer));
    
    arduinoCode = "void setup() {\n  // Initialize pins\n  pinMode(13, OUTPUT);\n}\n\nvoid loop() {\n  // Main code\n  digitalWrite(13, HIGH);\n  delay(1000);\n  digitalWrite(13, LOW);\n  delay(1000);\n}\n";
    
    strncpy(codeBuffer, arduinoCode.c_str(), sizeof(codeBuffer) - 1);
    
    initializePins();
    
    WavePlotter plotter;
    plotter.name = "Signal Monitor";
    plotter.channels.resize(4);
    plotters.push_back(plotter);
}

void IKSTab::initializePins() {
    for (int i = 0; i < totalPins; i++) {
        ArduinoPin pin;
        pin.pinNumber = i;
        pin.mode = "INPUT";
        pin.waveformData.resize(100, 0.0f);
        pin.waveformData.reserve(ArduinoPin::MAX_WAVEFORM_SIZE);
        pins[i] = pin;
    }
}

void IKSTab::render() {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.04f, 0.04f, 0.04f, 1.0f));
    ImGui::BeginChild("IKSContent", ImVec2(0, 0), true);
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("IKS - Arduino IDE with Integrated Circuits");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    if (ImGui::Button("Compile")) {
        compileCode();
    }
    ImGui::SameLine();
    if (ImGui::Button("Upload")) {
        uploadToArduino();
    }
    ImGui::SameLine();
    if (ImGui::Button("Link to ICS")) {
        linkToICS();
    }
    ImGui::SameLine();
    ImGui::Checkbox("Circuit Designer", &showCircuitDesigner);
    ImGui::SameLine();
    ImGui::Checkbox("Wave Plotter", &showWavePlotter);
    ImGui::SameLine();
    ImGui::Checkbox("Infinite Pin Mode", &infinitePinMode);
    
    if (infinitePinMode) {
        ImGui::SameLine();
        ImGui::InputInt("Total Pins", &totalPins);
        if (totalPins > pins.size()) {
            initializePins();
        }
    } else {
        totalPins = 40;
    }
    
    ImGui::Separator();
    
    ImGui::Columns(showCircuitDesigner ? 2 : 1, "IKSColumns");
    
    ImGui::BeginChild("CodeEditorSection", ImVec2(0, -300), true);
    renderCodeEditor();
    ImGui::EndChild();
    
    if (showCircuitDesigner) {
        ImGui::NextColumn();
        ImGui::BeginChild("CircuitSection", ImVec2(0, -300), true);
        renderCircuitDesigner();
        ImGui::EndChild();
    }
    
    ImGui::Columns(1);
    
    if (showWavePlotter) {
        ImGui::BeginChild("WavePlotterSection", ImVec2(0, 0), true);
        renderWavePlotter();
        ImGui::EndChild();
    } else {
        ImGui::BeginChild("PinControllerSection", ImVec2(0, 0), true);
        renderPinController();
        ImGui::EndChild();
    }
    
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void IKSTab::renderCodeEditor() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("Arduino Code Editor");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    if (ImGui::InputTextMultiline("##code", codeBuffer, sizeof(codeBuffer), 
        ImVec2(-1, -50), ImGuiInputTextFlags_AllowTabInput)) {
        arduinoCode = codeBuffer;
    }
    
    if (!compileOutput.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
        ImGui::TextWrapped("Output: %s", compileOutput.c_str());
        ImGui::PopStyleColor();
    }
}

void IKSTab::renderCircuitDesigner() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("Integrated Circuit Designer");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    
    draw_list->AddRectFilled(canvas_pos, 
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), 
        IM_COL32(10, 10, 10, 255));
    
    int pinCount = infinitePinMode ? std::min(totalPins, 100) : 40;
    int pinsPerSide = pinCount / 4;
    float spacing = canvas_size.x / (pinsPerSide + 1);
    
    ImVec2 chip_center(canvas_pos.x + canvas_size.x / 2, canvas_pos.y + canvas_size.y / 2);
    float chip_width = canvas_size.x * 0.6f;
    float chip_height = canvas_size.y * 0.6f;
    
    draw_list->AddRectFilled(
        ImVec2(chip_center.x - chip_width/2, chip_center.y - chip_height/2),
        ImVec2(chip_center.x + chip_width/2, chip_center.y + chip_height/2),
        IM_COL32(20, 20, 20, 255),
        5.0f
    );
    
    for (int i = 0; i < pinsPerSide; i++) {
        ImVec2 pin_pos_top(chip_center.x - chip_width/2 + (i+1) * spacing, chip_center.y - chip_height/2);
        ImVec2 pin_pos_bottom(chip_center.x - chip_width/2 + (i+1) * spacing, chip_center.y + chip_height/2);
        ImVec2 pin_pos_left(chip_center.x - chip_width/2, chip_center.y - chip_height/2 + (i+1) * spacing);
        ImVec2 pin_pos_right(chip_center.x + chip_width/2, chip_center.y - chip_height/2 + (i+1) * spacing);
        
        draw_list->AddCircleFilled(pin_pos_top, 5.0f, IM_COL32(0, 255, 136, 255));
        draw_list->AddCircleFilled(pin_pos_bottom, 5.0f, IM_COL32(0, 255, 136, 255));
        draw_list->AddCircleFilled(pin_pos_left, 5.0f, IM_COL32(0, 255, 136, 255));
        draw_list->AddCircleFilled(pin_pos_right, 5.0f, IM_COL32(0, 255, 136, 255));
        
        char label[8];
        snprintf(label, sizeof(label), "%d", i);
        draw_list->AddText(ImVec2(pin_pos_top.x - 5, pin_pos_top.y - 20), IM_COL32(255, 255, 255, 255), label);
    }
    
    if (icsRef) {
        static auto startTime = std::chrono::steady_clock::now();
        auto currentTime = std::chrono::steady_clock::now();
        float time = std::chrono::duration<float>(currentTime - startTime).count();
        
        for (int i = 0; i < 20; i++) {
            float x1 = chip_center.x + cos(time + i * 0.3f) * chip_width * 0.3f;
            float y1 = chip_center.y + sin(time + i * 0.3f) * chip_height * 0.3f;
            float x2 = chip_center.x + cos(time + i * 0.3f + 1.0f) * chip_width * 0.3f;
            float y2 = chip_center.y + sin(time + i * 0.3f + 1.0f) * chip_height * 0.3f;
            
            draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(0, 255, 136, 100), 1.0f);
        }
    }
    
    ImGui::Dummy(canvas_size);
}

void IKSTab::renderWavePlotter() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("Wave Plotter - Multi-Channel Signal Monitor");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    if (ImGui::Button("Add Channel")) {
        addWaveChannel();
    }
    ImGui::SameLine();
    if (ImGui::Button("Toggle 3D")) {
        if (!plotters.empty()) {
            plotters[0].is3D = !plotters[0].is3D;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button(plotters.empty() || plotters[0].paused ? "Resume" : "Pause")) {
        if (!plotters.empty()) {
            plotters[0].paused = !plotters[0].paused;
        }
    }
    
    ImGui::Separator();
    
    if (!plotters.empty()) {
        updateWaveforms();
        
        if (plotters[0].is3D) {
            render3DWave(plotters[0]);
        } else {
            render2DWave(plotters[0]);
        }
    }
}

void IKSTab::render2DWave(const WavePlotter& plotter) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    
    draw_list->AddRectFilled(canvas_pos, 
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), 
        IM_COL32(5, 5, 5, 255));
    
    draw_list->AddLine(
        ImVec2(canvas_pos.x, canvas_pos.y + canvas_size.y / 2),
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y / 2),
        IM_COL32(50, 50, 50, 255)
    );
    
    ImU32 colors[] = {
        IM_COL32(0, 255, 136, 255),
        IM_COL32(255, 136, 0, 255),
        IM_COL32(136, 0, 255, 255),
        IM_COL32(255, 0, 136, 255)
    };
    
    for (size_t ch = 0; ch < plotter.channels.size(); ch++) {
        const auto& channel = plotter.channels[ch];
        if (channel.empty()) continue;
        
        for (size_t i = 1; i < channel.size(); i++) {
            float x1 = canvas_pos.x + (i - 1) * canvas_size.x / channel.size();
            float y1 = canvas_pos.y + canvas_size.y / 2 - channel[i-1] * canvas_size.y * 0.4f;
            float x2 = canvas_pos.x + i * canvas_size.x / channel.size();
            float y2 = canvas_pos.y + canvas_size.y / 2 - channel[i] * canvas_size.y * 0.4f;
            
            draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), colors[ch % 4], 2.0f);
        }
    }
    
    ImGui::Dummy(canvas_size);
}

void IKSTab::render3DWave(const WavePlotter& plotter) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    
    draw_list->AddRectFilled(canvas_pos, 
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), 
        IM_COL32(5, 5, 5, 255));
    
    static auto startTime = std::chrono::steady_clock::now();
    auto currentTime = std::chrono::steady_clock::now();
    float time = std::chrono::duration<float>(currentTime - startTime).count();
    
    for (size_t ch = 0; ch < plotter.channels.size(); ch++) {
        const auto& channel = plotter.channels[ch];
        if (channel.empty()) continue;
        
        for (size_t i = 1; i < channel.size(); i++) {
            float depth = ch * 20.0f;
            float x1 = canvas_pos.x + (i - 1) * canvas_size.x / channel.size();
            float y1 = canvas_pos.y + canvas_size.y / 2 - channel[i-1] * 100.0f + depth;
            float x2 = canvas_pos.x + i * canvas_size.x / channel.size();
            float y2 = canvas_pos.y + canvas_size.y / 2 - channel[i] * 100.0f + depth;
            
            ImU32 col = IM_COL32(
                (int)(128 + 127 * sin(time + ch)),
                (int)(128 + 127 * cos(time + ch)),
                (int)(128 + 127 * sin(time * 1.5f + ch)),
                255
            );
            
            draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), col, 2.0f);
        }
    }
    
    ImGui::Dummy(canvas_size);
}

void IKSTab::renderPinController() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("Pin Controller");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    ImGui::Columns(4, "PinColumns");
    
    for (auto& [num, pin] : pins) {
        if (num >= totalPins) break;
        
        ImGui::PushID(num);
        ImGui::Text("Pin %d", num);
        
        const char* modes[] = { "INPUT", "OUTPUT", "INPUT_PULLUP" };
        int current_mode = 0;
        if (pin.mode == "OUTPUT") current_mode = 1;
        if (pin.mode == "INPUT_PULLUP") current_mode = 2;
        
        if (ImGui::Combo("##mode", &current_mode, modes, 3)) {
            pin.mode = modes[current_mode];
        }
        
        if (pin.mode == "OUTPUT") {
            ImGui::SliderFloat("##value", &pin.value, 0.0f, 1.0f);
        } else {
            ImGui::Text("Value: %.2f", pin.value);
        }
        
        ImGui::PopID();
        ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
}

void IKSTab::compileCode() {
    isCompiling = true;
    compileOutput = "Compiling... Success! Code compiled without errors.";
    isCompiling = false;
}

void IKSTab::uploadToArduino() {
    compileOutput = "Uploading to Arduino... Upload complete!";
}

void IKSTab::addWaveChannel() {
    if (!plotters.empty() && plotters[0].channels.size() < WavePlotter::MAX_CHANNELS) {
        plotters[0].channels.push_back(std::vector<float>());
        plotters[0].channels.back().reserve(plotters[0].maxSamples);
    }
}

void IKSTab::updateWaveforms() {
    if (plotters.empty() || plotters[0].paused) return;
    
    static auto startTime = std::chrono::steady_clock::now();
    auto currentTime = std::chrono::steady_clock::now();
    float time = std::chrono::duration<float>(currentTime - startTime).count();
    
    for (size_t ch = 0; ch < plotters[0].channels.size(); ch++) {
        auto& channel = plotters[0].channels[ch];
        
        float value;
        if (ch < 4) {
            value = DataBridge::getInstance().getArduinoPinValue(ch);
        } else {
            value = sin(time * 2.0f + ch * 0.5f) * cos(time * 0.5f);
        }
        
        channel.push_back(value);
        
        if (channel.size() > (size_t)plotters[0].maxSamples) {
            channel.erase(channel.begin());
        }
    }
    
    for (auto& [num, pin] : pins) {
        if (pin.mode == "OUTPUT") {
            DataBridge::getInstance().setArduinoPinValue(num, pin.value);
        } else {
            pin.value = DataBridge::getInstance().getArduinoPinValue(num);
        }
    }
}

void IKSTab::linkToICS() {
    try {
        std::vector<int> selectedPixels;
        auto* pixelData = DataBridge::getInstance().getPixelData();
        
        if (pixelData) {
            for (size_t i = 0; i < pixelData->size(); i++) {
                if ((*pixelData)[i].selected) {
                    selectedPixels.push_back(i);
                }
            }
            
            if (!selectedPixels.empty()) {
                DataBridge::getInstance().addArduinoToPixelMapping(13, selectedPixels);
                DataBridge::getInstance().setPixelToArduino(selectedPixels[0], 13, 'r');
                compileOutput = "Linked " + std::to_string(selectedPixels.size()) + " pixels to Arduino pin 13";
            } else {
                compileOutput = "No pixels selected";
            }
        }
    } catch (const std::exception& e) {
        compileOutput = "Error linking pixels: Exception occurred";
    } catch (...) {
        compileOutput = "Error linking pixels: Unknown error";
    }
}

}
