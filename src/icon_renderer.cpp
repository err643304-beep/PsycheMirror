#include "icon_renderer.hpp"
#include <vector>
#include <cstring>

void IconRenderer::renderSVGIcon(const char* svgPath, ImVec2 pos, ImVec2 size, ImU32 color) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    parseSVGPath(svgPath, drawList, pos, size, color);
}

void IconRenderer::renderIconButton(const char* label, const char* svgPath, bool selected) {
    ImVec2 buttonSize(60, 50);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    
    ImU32 bgColor = selected ? IM_COL32(60, 100, 180, 255) : IM_COL32(40, 40, 50, 0);
    ImU32 hoverColor = IM_COL32(50, 80, 150, 200);
    ImU32 iconColor = selected ? IM_COL32(255, 255, 255, 255) : IM_COL32(180, 180, 200, 255);
    
    ImGui::PushID(label);
    
    bool hovered = ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x + buttonSize.x, pos.y + buttonSize.y));
    if (hovered && !selected) {
        bgColor = hoverColor;
        iconColor = IM_COL32(255, 255, 255, 255);
    }
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(pos, ImVec2(pos.x + buttonSize.x, pos.y + buttonSize.y), bgColor, 4.0f);
    
    if (selected) {
        drawList->AddRect(pos, ImVec2(pos.x + buttonSize.x, pos.y + buttonSize.y), 
            IM_COL32(100, 150, 255, 255), 4.0f, 0, 2.0f);
    }
    
    ImVec2 iconPos = ImVec2(pos.x + (buttonSize.x - 24) / 2, pos.y + 8);
    ImVec2 iconSize(24, 24);
    renderSVGIcon(svgPath, iconPos, iconSize, iconColor);
    
    ImVec2 textSize = ImGui::CalcTextSize(label);
    ImVec2 textPos = ImVec2(pos.x + (buttonSize.x - textSize.x) / 2, pos.y + buttonSize.y - 18);
    drawList->AddText(textPos, iconColor, label);
    
    ImGui::InvisibleButton(label, buttonSize);
    
    ImGui::PopID();
}

void IconRenderer::parseSVGPath(const char* pathData, ImDrawList* drawList, ImVec2 offset, ImVec2 scale, ImU32 color) {
    drawList->AddCircleFilled(
        ImVec2(offset.x + scale.x / 2, offset.y + scale.y / 2),
        scale.x / 3,
        color,
        16
    );
}
