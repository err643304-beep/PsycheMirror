#include "editor_tab.hpp"

namespace Tabs {

void EditorTab::render() {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.15f, 1.0f));
    ImGui::BeginChild("EditorContent", ImVec2(0, 0), true);
    
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Code Editor");
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::BeginGroup();
    ImGui::Text("Language:");
    ImGui::SameLine();
    const char* languages[] = {"C++", "Python", "JavaScript", "GLSL"};
    static int lang_idx = 0;
    ImGui::SetNextItemWidth(150);
    ImGui::Combo("##Language", &lang_idx, languages, IM_ARRAYSIZE(languages));
    ImGui::SameLine();
    
    ImGui::SameLine(ImGui::GetWindowWidth() - 120);
    if (ImGui::Button("Run Code", ImVec2(100, 0))) {
    }
    ImGui::EndGroup();
    
    ImGui::Spacing();
    
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.08f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
    ImGui::InputTextMultiline(
        "##CodeEditor",
        &codeBuffer,
        ImVec2(-1, ImGui::GetWindowHeight() - 150),
        ImGuiInputTextFlags_AllowTabInput
    );
    ImGui::PopStyleColor(2);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Output:");
    ImGui::BeginChild("OutputPanel", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::TextWrapped("Console output will appear here...");
    ImGui::EndChild();
    
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

}
