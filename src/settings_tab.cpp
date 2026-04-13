#include "settings_tab.hpp"
#include "../../../third_party/imgui/misc/cpp/imgui_stdlib.h"

namespace Tabs {

SettingsTab::SettingsTab() {
    manualCodeBuffer = "// Enter your custom Qt or C++ code here\n";
}

void SettingsTab::showQtIntegrationDialog() {
}

void SettingsTab::showManualCodeEditor() {
}

void SettingsTab::render() {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.15f, 1.0f));
    ImGui::BeginChild("SettingsContent", ImVec2(0, 0), true);
    
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Settings");
    ImGui::Separator();
    ImGui::Spacing();
    
    if (ImGui::CollapsingHeader("Appearance", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent();
        ImGui::Checkbox("Dark Mode", &darkModeEnabled);
        
        ImGui::Text("Theme Color:");
        static float color[3] = {0.4f, 0.6f, 0.8f};
        ImGui::ColorEdit3("##ThemeColor", color);
        
        ImGui::Text("Font Size:");
        static int fontSize = 16;
        ImGui::SliderInt("##FontSize", &fontSize, 10, 24);
        
        ImGui::Unindent();
        ImGui::Spacing();
    }
    
    if (ImGui::CollapsingHeader("Qt Integration", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent();
        ImGui::Checkbox("Enable Qt Widgets", &qtIntegrationEnabled);
        
        if (qtIntegrationEnabled) {
            ImGui::Spacing();
            if (ImGui::Button("Open Qt Widget Demo", ImVec2(200, 0))) {
                showQtIntegrationDialog();
            }
            ImGui::SameLine();
            ImGui::TextDisabled("(?)");
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Opens a Qt dialog window");
            }
        }
        
        ImGui::Unindent();
        ImGui::Spacing();
    }
    
    if (ImGui::CollapsingHeader("Manual Code Editor", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent();
        ImGui::TextWrapped("Edit and test custom C++/Qt code directly:");
        ImGui::Spacing();
        
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.08f, 0.1f, 1.0f));
        ImGui::InputTextMultiline(
            "##ManualCode",
            &manualCodeBuffer,
            ImVec2(-1, 200),
            ImGuiInputTextFlags_AllowTabInput
        );
        ImGui::PopStyleColor();
        
        ImGui::Spacing();
        if (ImGui::Button("Compile & Run", ImVec2(150, 0))) {
            showManualCodeEditor();
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear", ImVec2(100, 0))) {
            manualCodeBuffer = "// Enter your custom Qt or C++ code here\n";
        }
        
        ImGui::Unindent();
        ImGui::Spacing();
    }
    
    if (ImGui::CollapsingHeader("Advanced")) {
        ImGui::Indent();
        ImGui::Text("OpenGL Version: 3.3");
        ImGui::Text("ImGui Version: %s", ImGui::GetVersion());
        ImGui::Text("Qt Version: 5/6");
        
        ImGui::Spacing();
        if (ImGui::Button("Reset All Settings")) {
            qtIntegrationEnabled = false;
            darkModeEnabled = false;
        }
        
        ImGui::Unindent();
    }
    
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

}
