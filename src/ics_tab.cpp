#include "ics_tab.hpp"
#include "data_bridge.hpp"
#include <cmath>
#include <algorithm>
#include <chrono>

namespace Tabs {

ICSTab::ICSTab() 
    : zoom(1.0f), scrollOffset(0, 0), showGrid(true), showCoordinates(false), threadRunning(false) {
    initializePixels();
    DataBridge::getInstance().setPixelData(&pixels);
    startRenderThread();
}

ICSTab::~ICSTab() {
    stopRenderThread();
    DataBridge::getInstance().setPixelData(nullptr);
}

void ICSTab::initializePixels() {
    pixels.clear();
    pixels.reserve(GRID_WIDTH * GRID_HEIGHT);
    
    for (int row = 0; row < GRID_HEIGHT; row++) {
        for (int col = 0; col < GRID_WIDTH; col++) {
            SubPixel p;
            p.row = row;
            p.col = col;
            p.x = col * PIXEL_SIZE;
            p.y = row * PIXEL_SIZE;
            p.r = (float)col / GRID_WIDTH;
            p.g = (float)row / GRID_HEIGHT;
            p.b = 0.5f;
            p.a = 1.0f;
            pixels.push_back(p);
        }
    }
}

void ICSTab::startRenderThread() {
    threadRunning = true;
    renderThread = std::thread([this]() {
        auto startTime = std::chrono::steady_clock::now();
        while (threadRunning) {
            try {
                auto currentTime = std::chrono::steady_clock::now();
                float time = std::chrono::duration<float>(currentTime - startTime).count();
                
                {
                    std::lock_guard<std::mutex> lock(pixelMutex);
                    for (auto& pixel : pixels) {
                        if (!pixel.equation.empty()) {
                            updatePixelFromEquation(pixel, time);
                        } else {
                            float wave = sin(time + pixel.col * 0.1f) * 0.5f + 0.5f;
                            pixel.r = wave;
                            pixel.g = cos(time + pixel.row * 0.1f) * 0.5f + 0.5f;
                            pixel.b = sin(time * 2.0f) * 0.5f + 0.5f;
                        }
                    }
                    DataBridge::getInstance().updateArduinoFromPixels();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(33));
            } catch (...) {
            }
        }
    });
}

void ICSTab::stopRenderThread() {
    threadRunning = false;
    if (renderThread.joinable()) {
        renderThread.join();
    }
}

void ICSTab::updatePixelFromEquation(SubPixel& pixel, float time) {
    float t = time;
    float x = pixel.col / (float)GRID_WIDTH;
    float y = pixel.row / (float)GRID_HEIGHT;
    pixel.r = sin(t + x * 3.14159f) * 0.5f + 0.5f;
    pixel.g = cos(t + y * 3.14159f) * 0.5f + 0.5f;
    pixel.b = sin(t * 2.0f + (x + y)) * 0.5f + 0.5f;
}

void ICSTab::render() {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.04f, 0.04f, 0.04f, 1.0f));
    ImGui::BeginChild("ICSContent", ImVec2(0, 0), true);
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("ICS - Inline Sub Pixels Renderer");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    ImGui::Checkbox("Show Grid", &showGrid);
    ImGui::SameLine();
    ImGui::Checkbox("Show Coordinates", &showCoordinates);
    ImGui::SameLine();
    ImGui::SliderFloat("Zoom", &zoom, 0.5f, 5.0f);
    
    if (ImGui::Button("Export Selected to ECI")) {
        exportSelectedToECI();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear Selection")) {
        selectedPixels.clear();
        for (auto& p : pixels) p.selected = false;
    }
    
    ImGui::Text("Selected Pixels: %zu", selectedPixels.size());
    ImGui::SameLine();
    
    auto& arduinoMappings = DataBridge::getInstance().getArduinoMappings();
    auto& pixelMappings = DataBridge::getInstance().getPixelMappings();
    ImGui::Text("| Arduino Links: %zu pixel→pin, %zu pin→pixel", 
        pixelMappings.size(), arduinoMappings.size());
    
    ImGui::Separator();
    
    renderPixelGrid();
    renderPixelData();
    
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void ICSTab::renderPixelGrid() {
    ImGui::BeginChild("PixelGridView", ImVec2(0, -150), true);
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    
    float scaled_size = PIXEL_SIZE * zoom;
    
    {
        std::lock_guard<std::mutex> lock(pixelMutex);
        for (const auto& pixel : pixels) {
            if (!pixel.active) continue;
            
            ImVec2 p_min(canvas_pos.x + pixel.x * zoom, canvas_pos.y + pixel.y * zoom);
            ImVec2 p_max(p_min.x + scaled_size, p_min.y + scaled_size);
            
            ImU32 col = IM_COL32(
                (int)(pixel.r * 255),
                (int)(pixel.g * 255),
                (int)(pixel.b * 255),
                (int)(pixel.a * 255)
            );
            
            draw_list->AddRectFilled(p_min, p_max, col);
            
            if (pixel.selected) {
                draw_list->AddRect(p_min, p_max, IM_COL32(0, 255, 136, 255), 0.0f, 0, 2.0f);
            }
            
            bool hasArduinoLink = false;
            auto& pixelMappings = DataBridge::getInstance().getPixelMappings();
            for (auto& mapping : pixelMappings) {
                if (mapping.pixelIndex == static_cast<int>(&pixel - &pixels[0])) {
                    hasArduinoLink = true;
                    break;
                }
            }
            
            if (hasArduinoLink) {
                draw_list->AddCircleFilled(
                    ImVec2(p_min.x + scaled_size/2, p_min.y + scaled_size/2), 
                    scaled_size * 0.2f, 
                    IM_COL32(255, 136, 0, 200)
                );
            }
            
            if (showGrid && zoom > 2.0f) {
                draw_list->AddRect(p_min, p_max, IM_COL32(50, 50, 50, 100));
            }
        }
    }
    
    handlePixelSelection();
    
    ImGui::EndChild();
}

void ICSTab::handlePixelSelection() {
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
        ImVec2 mouse_pos = ImGui::GetMousePos();
        ImVec2 canvas_pos = ImGui::GetItemRectMin();
        
        float scaled_size = PIXEL_SIZE * zoom;
        if (scaled_size <= 0.0f) return;
        
        int col = (int)((mouse_pos.x - canvas_pos.x) / scaled_size);
        int row = (int)((mouse_pos.y - canvas_pos.y) / scaled_size);
        
        if (col >= 0 && col < GRID_WIDTH && row >= 0 && row < GRID_HEIGHT) {
            int idx = row * GRID_WIDTH + col;
            if (idx >= 0 && idx < pixels.size()) {
                std::lock_guard<std::mutex> lock(pixelMutex);
                pixels[idx].selected = !pixels[idx].selected;
                
                if (pixels[idx].selected) {
                    selectedPixels.push_back(idx);
                } else {
                    selectedPixels.erase(
                        std::remove(selectedPixels.begin(), selectedPixels.end(), idx),
                        selectedPixels.end()
                    );
                }
            }
        }
    }
}

void ICSTab::renderPixelData() {
    ImGui::BeginChild("PixelDataPanel", ImVec2(0, 0), true);
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("Pixel Data Inspector");
    ImGui::PopStyleColor();
    
    std::lock_guard<std::mutex> lock(pixelMutex);
    if (!selectedPixels.empty() && selectedPixels[0] >= 0 && selectedPixels[0] < pixels.size()) {
        SubPixel& p = pixels[selectedPixels[0]];
        
        ImGui::Text("Row: %d, Col: %d", p.row, p.col);
        ImGui::ColorEdit4("Color", &p.r);
        
        char buf[256];
        strncpy(buf, p.equation.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("Equation", buf, sizeof(buf))) {
            p.equation = buf;
        }
    } else {
        ImGui::TextDisabled("Select a pixel to inspect");
    }
    
    ImGui::EndChild();
}

void ICSTab::exportSelectedToECI() {
}

}
