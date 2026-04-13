#include "home_tab.hpp"
#include <cmath>

namespace Tabs {

void HomeTab::render() {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.15f, 1.0f));
    ImGui::BeginChild("HomeContent", ImVec2(0, 0), true);
    
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    
    ImVec2 center = ImVec2(ImGui::GetWindowWidth() / 2, 200);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    
    float time = (float)ImGui::GetTime();
    for (int i = 0; i < 3; i++) {
        float angle = time * 2.0f + i * 2.0f * 3.14159f / 3.0f;
        float x = center.x + std::cos(angle) * 80.0f;
        float y = center.y + std::sin(angle) * 80.0f;
        ImU32 col = IM_COL32(
            (int)(127 + 127 * std::sin(time + i)),
            (int)(127 + 127 * std::cos(time + i * 2)),
            (int)(127 + 127 * std::sin(time * 1.5f + i)),
            255
        );
        draw_list->AddCircleFilled(ImVec2(p.x + x, p.y + y), 20.0f, col, 32);
    }
    
    ImGui::Dummy(ImVec2(0, 220));
    
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 400) / 2);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::TextWrapped("Welcome to PyWGL");
    ImGui::PopStyleColor();
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 500) / 2);
    ImGui::BeginGroup();
    
    ImGui::BulletText("Integrated OpenGL and ImGui rendering");
    ImGui::BulletText("Python integration for dynamic scripting");
    ImGui::BulletText("Qt widgets support for advanced UI");
    ImGui::BulletText("Real-time code editing and execution");
    ImGui::BulletText("3D viewer with interactive controls");
    
    ImGui::EndGroup();
    
    ImGui::Spacing();
    ImGui::Spacing();
    
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 200) / 2);
    if (ImGui::Button("Get Started", ImVec2(200, 40))) {
    }
    
    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

}
