#include "lsi_tab.hpp"
#include <algorithm>

namespace Tabs {

LSITab::LSITab() 
    : icsRef(nullptr), hoveredPixelIndex(-1), contextMenuPixelIndex(-1), 
      showContextMenu(false), globalDisplaySize(50.0f), showLabels(true), showValues(true) {
}

void LSITab::render() {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.04f, 0.04f, 0.04f, 1.0f));
    ImGui::BeginChild("LSIContent", ImVec2(0, 0), true);
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
    ImGui::Text("LSI - Large Subpixels View");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    if (ImGui::Button("Sync with ICS")) {
        syncWithICS();
    }
    ImGui::SameLine();
    if (ImGui::Button("Export to IKS")) {
        exportToIKS();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear All")) {
        largePixels.clear();
    }
    
    ImGui::SliderFloat("Display Size", &globalDisplaySize, 30.0f, 150.0f);
    ImGui::SameLine();
    ImGui::Checkbox("Show Labels", &showLabels);
    ImGui::SameLine();
    ImGui::Checkbox("Show Values", &showValues);
    
    ImGui::Text("Large Pixels: %zu", largePixels.size());
    
    ImGui::Separator();
    
    renderLargePixelView();
    
    if (showContextMenu) {
        renderContextMenu();
    }
    
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void LSITab::renderLargePixelView() {
    ImGui::BeginChild("LargePixelCanvas", ImVec2(0, 0), true);
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
    
    float spacing = 10.0f;
    int columns = (int)(ImGui::GetContentRegionAvail().x / (globalDisplaySize + spacing));
    if (columns < 1) columns = 1;
    
    for (size_t i = 0; i < largePixels.size(); i++) {
        LargeSubPixel& lp = largePixels[i];
        
        if (lp.pixelIndex < 0 || !icsRef) continue;
        
        std::lock_guard<std::mutex> lock(icsRef->getPixelMutex());
        auto& pixels = icsRef->getPixels();
        
        if (lp.pixelIndex >= pixels.size()) continue;
        
        SubPixel& pixel = pixels[lp.pixelIndex];
        
        int row = i / columns;
        int col = i % columns;
        
        ImVec2 pos(
            cursor_pos.x + col * (globalDisplaySize + spacing),
            cursor_pos.y + row * (globalDisplaySize + spacing)
        );
        
        ImVec2 p_max(pos.x + globalDisplaySize, pos.y + globalDisplaySize);
        
        ImU32 pixel_col = IM_COL32(
            (int)(pixel.r * 255),
            (int)(pixel.g * 255),
            (int)(pixel.b * 255),
            (int)(pixel.a * 255)
        );
        
        draw_list->AddRectFilled(pos, p_max, pixel_col, 4.0f);
        draw_list->AddRect(pos, p_max, IM_COL32(0, 255, 136, 180), 4.0f, 0, 2.0f);
        
        ImGui::SetCursorScreenPos(pos);
        ImGui::InvisibleButton(("##largepixel" + std::to_string(i)).c_str(), 
            ImVec2(globalDisplaySize, globalDisplaySize));
        
        if (ImGui::IsItemHovered()) {
            hoveredPixelIndex = i;
            
            if (ImGui::IsMouseClicked(1)) {
                contextMenuPixelIndex = i;
                showContextMenu = true;
            }
        }
        
        if (showLabels) {
            ImVec2 label_pos(pos.x + 5, pos.y + 5);
            draw_list->AddText(label_pos, IM_COL32(255, 255, 255, 255), 
                ("R" + std::to_string(lp.row) + " C" + std::to_string(lp.col)).c_str());
        }
        
        if (showValues) {
            char val_buf[64];
            snprintf(val_buf, sizeof(val_buf), "R:%.2f\nG:%.2f\nB:%.2f", 
                pixel.r, pixel.g, pixel.b);
            ImVec2 val_pos(pos.x + 5, pos.y + globalDisplaySize - 50);
            draw_list->AddText(val_pos, IM_COL32(255, 255, 255, 255), val_buf);
        }
    }
    
    ImGui::EndChild();
}

void LSITab::renderContextMenu() {
    if (ImGui::BeginPopupContextVoid("LSIContextMenu")) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.53f, 1.0f));
        ImGui::Text("Pixel Operations");
        ImGui::PopStyleColor();
        
        ImGui::Separator();
        
        if (ImGui::MenuItem("Add Equation")) {
            if (contextMenuPixelIndex >= 0 && contextMenuPixelIndex < largePixels.size()) {
            }
        }
        
        if (ImGui::MenuItem("Add Sub-Pixels")) {
        }
        
        if (ImGui::MenuItem("Apply Algorithm")) {
        }
        
        if (ImGui::MenuItem("Export to IKS")) {
            if (contextMenuPixelIndex >= 0 && contextMenuPixelIndex < largePixels.size()) {
                exportToIKS();
            }
        }
        
        ImGui::Separator();
        
        if (ImGui::MenuItem("Remove from View")) {
            if (contextMenuPixelIndex >= 0 && contextMenuPixelIndex < largePixels.size()) {
                removePixelFromView(contextMenuPixelIndex);
            }
        }
        
        ImGui::EndPopup();
    } else {
        showContextMenu = false;
    }
    
    if (showContextMenu) {
        ImGui::OpenPopup("LSIContextMenu");
    }
}

void LSITab::syncWithICS() {
    try {
        if (!icsRef) return;
        
        largePixels.clear();
        
        std::lock_guard<std::mutex> lock(icsRef->getPixelMutex());
        auto& pixels = icsRef->getPixels();
        
        for (size_t i = 0; i < pixels.size(); i++) {
            if (pixels[i].selected) {
                addPixelToView(i);
            }
        }
    } catch (const std::exception& e) {
    } catch (...) {
    }
}

void LSITab::addPixelToView(int pixelIndex) {
    if (!icsRef) return;
    
    auto& pixels = icsRef->getPixels();
    if (pixelIndex < 0 || pixelIndex >= pixels.size()) return;
    
    LargeSubPixel lp;
    lp.pixelIndex = pixelIndex;
    lp.row = pixels[pixelIndex].row;
    lp.col = pixels[pixelIndex].col;
    lp.displaySize = globalDisplaySize;
    
    largePixels.push_back(lp);
}

void LSITab::removePixelFromView(int index) {
    if (index >= 0 && index < largePixels.size()) {
        largePixels.erase(largePixels.begin() + index);
    }
}

void LSITab::exportToIKS() {
}

}
