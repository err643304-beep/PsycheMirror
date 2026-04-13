#include "navbar.hpp"

NavBar::NavBar() : activeTab(TabType::HOME) {
    homeTab = std::make_unique<Tabs::HomeTab>();
    editorTab = std::make_unique<Tabs::EditorTab>();
    viewerTab = std::make_unique<Tabs::ViewerTab>();
    settingsTab = std::make_unique<Tabs::SettingsTab>();
    icsTab = std::make_unique<Tabs::ICSTab>();
    csiTab = std::make_unique<Tabs::CSITab>();
    lsiTab = std::make_unique<Tabs::LSITab>();
    eikosTab = std::make_unique<Tabs::EIKOSTab>();
    iksTab = std::make_unique<Tabs::IKSTab>();
    
    csiTab->setICSReference(icsTab.get());
    lsiTab->setICSReference(icsTab.get());
    eikosTab->setICSReference(icsTab.get());
    iksTab->setICSReference(icsTab.get());
}

NavBar::~NavBar() {
    csiTab.reset();
    lsiTab.reset();
    eikosTab.reset();
    iksTab.reset();
    icsTab.reset();
    homeTab.reset();
    editorTab.reset();
    viewerTab.reset();
    settingsTab.reset();
}

void NavBar::render() {
    renderNavBar();
    renderActiveTab();
}

void NavBar::renderNavBar() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 60));
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 5));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.18f, 1.0f));
    
    ImGui::Begin("##NavBar", nullptr, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings);
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));
    
    IconRenderer::renderIconButton("Home", homeTab->getIcon(), activeTab == TabType::HOME);
    if (ImGui::IsItemClicked()) {
        activeTab = TabType::HOME;
    }
    
    ImGui::SameLine();
    IconRenderer::renderIconButton("Editor", editorTab->getIcon(), activeTab == TabType::EDITOR);
    if (ImGui::IsItemClicked()) {
        activeTab = TabType::EDITOR;
    }
    
    ImGui::SameLine();
    IconRenderer::renderIconButton("Viewer", viewerTab->getIcon(), activeTab == TabType::VIEWER);
    if (ImGui::IsItemClicked()) {
        activeTab = TabType::VIEWER;
    }
    
    ImGui::SameLine();
    IconRenderer::renderIconButton("Settings", settingsTab->getIcon(), activeTab == TabType::SETTINGS);
    if (ImGui::IsItemClicked()) {
        activeTab = TabType::SETTINGS;
    }
    
    ImGui::SameLine();
    IconRenderer::renderIconButton("ICS", icsTab->getIcon(), activeTab == TabType::ICS);
    if (ImGui::IsItemClicked()) {
        activeTab = TabType::ICS;
    }
    
    ImGui::SameLine();
    IconRenderer::renderIconButton("CSI", csiTab->getIcon(), activeTab == TabType::CSI);
    if (ImGui::IsItemClicked()) {
        activeTab = TabType::CSI;
    }
    
    ImGui::SameLine();
    IconRenderer::renderIconButton("LSI", lsiTab->getIcon(), activeTab == TabType::LSI);
    if (ImGui::IsItemClicked()) {
        activeTab = TabType::LSI;
    }
    
    ImGui::SameLine();
    IconRenderer::renderIconButton("EIKOS", eikosTab->getIcon(), activeTab == TabType::EIKOS);
    if (ImGui::IsItemClicked()) {
        activeTab = TabType::EIKOS;
    }
    
    ImGui::SameLine();
    IconRenderer::renderIconButton("IKS", iksTab->getIcon(), activeTab == TabType::IKS);
    if (ImGui::IsItemClicked()) {
        activeTab = TabType::IKS;
    }
    
    ImGui::PopStyleVar();
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}

void NavBar::renderActiveTab() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + 60));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - 60));
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.12f, 1.0f));
    
    ImGui::Begin("##MainContent", nullptr, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoSavedSettings);
    
    switch (activeTab) {
        case TabType::HOME:
            homeTab->render();
            break;
        case TabType::EDITOR:
            editorTab->render();
            break;
        case TabType::VIEWER:
            viewerTab->render();
            break;
        case TabType::SETTINGS:
            settingsTab->render();
            break;
        case TabType::ICS:
            icsTab->render();
            break;
        case TabType::CSI:
            csiTab->render();
            break;
        case TabType::LSI:
            lsiTab->render();
            break;
        case TabType::EIKOS:
            eikosTab->render();
            break;
        case TabType::IKS:
            iksTab->render();
            break;
    }
    
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}
