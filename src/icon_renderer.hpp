#pragma once

#include "imgui.h"
#include <string>

class IconRenderer {
public:
    static void renderSVGIcon(const char* svgPath, ImVec2 pos, ImVec2 size, ImU32 color);
    static void renderIconButton(const char* label, const char* svgPath, bool selected);
    
private:
    static void parseSVGPath(const char* pathData, ImDrawList* drawList, ImVec2 offset, ImVec2 scale, ImU32 color);
};
