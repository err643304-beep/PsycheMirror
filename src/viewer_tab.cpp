#include "viewer_tab.hpp"
#include <cmath>

namespace Tabs {

static void draw_3d_cube(ImDrawList* draw_list, ImVec2 center, float size, float rotation) {
    float vertices[8][3] = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}
    };
    
    float cos_a = std::cos(rotation);
    float sin_a = std::sin(rotation);
    float cos_b = std::cos(rotation * 0.7f);
    float sin_b = std::sin(rotation * 0.7f);
    
    ImVec2 projected[8];
    for (int i = 0; i < 8; i++) {
        float x = vertices[i][0];
        float y = vertices[i][1];
        float z = vertices[i][2];
        
        float x1 = x * cos_a - z * sin_a;
        float z1 = x * sin_a + z * cos_a;
        
        float y1 = y * cos_b - z1 * sin_b;
        float z2 = y * sin_b + z1 * cos_b;
        
        float scale = 3.0f / (z2 + 4.0f);
        projected[i] = ImVec2(center.x + x1 * scale * size, center.y + y1 * scale * size);
    }
    
    ImU32 colors[] = {
        IM_COL32(255, 80, 80, 255), 
        IM_COL32(80, 255, 80, 255), 
        IM_COL32(80, 80, 255, 255), 
        IM_COL32(255, 255, 80, 255)
    };
    
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };
    
    for (int i = 0; i < 12; i++) {
        draw_list->AddLine(
            projected[edges[i][0]], 
            projected[edges[i][1]], 
            colors[i % 4], 
            3.0f
        );
    }
    
    for (int i = 0; i < 8; i++) {
        draw_list->AddCircleFilled(projected[i], 5.0f, IM_COL32(255, 255, 255, 255));
    }
}

void ViewerTab::render() {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.15f, 1.0f));
    ImGui::BeginChild("ViewerContent", ImVec2(0, 0), true);
    
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "3D Viewer");
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::BeginGroup();
    ImGui::Text("Rotation Speed:");
    ImGui::SameLine();
    static float speed = 1.0f;
    ImGui::SetNextItemWidth(200);
    ImGui::SliderFloat("##Speed", &speed, 0.0f, 5.0f);
    
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        rotation = 0.0f;
        speed = 1.0f;
    }
    ImGui::EndGroup();
    
    ImGui::Spacing();
    
    rotation += speed * 0.02f;
    
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    ImVec2 center = ImVec2(canvas_pos.x + canvas_size.x / 2, canvas_pos.y + canvas_size.y / 2);
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_pos, 
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
        IM_COL32(20, 20, 25, 255));
    
    draw_3d_cube(draw_list, center, 100.0f, rotation);
    
    ImGui::Dummy(canvas_size);
    
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

}
