#pragma once

#include "imgui.h"
#include "home_tab.hpp"
#include "editor_tab.hpp"
#include "viewer_tab.hpp"
#include "settings_tab.hpp"
#include "ics_tab.hpp"
#include "csi_tab.hpp"
#include "lsi_tab.hpp"
#include "eikos_tab.hpp"
#include "iks_tab.hpp"
#include "icon_renderer.hpp"
#include <memory>
#include <vector>

enum class TabType {
    HOME,
    EDITOR,
    VIEWER,
    SETTINGS,
    ICS,
    CSI,
    LSI,
    EIKOS,
    IKS
};

class NavBar {
public:
    NavBar();
    ~NavBar();

    void render();
    void renderNavBar();
    void renderActiveTab();
    
    TabType getActiveTab() const { return activeTab; }
    void setActiveTab(TabType tab) { activeTab = tab; }

private:
    TabType activeTab;
    
    std::unique_ptr<Tabs::HomeTab> homeTab;
    std::unique_ptr<Tabs::EditorTab> editorTab;
    std::unique_ptr<Tabs::ViewerTab> viewerTab;
    std::unique_ptr<Tabs::SettingsTab> settingsTab;
    std::unique_ptr<Tabs::ICSTab> icsTab;
    std::unique_ptr<Tabs::CSITab> csiTab;
    std::unique_ptr<Tabs::LSITab> lsiTab;
    std::unique_ptr<Tabs::EIKOSTab> eikosTab;
    std::unique_ptr<Tabs::IKSTab> iksTab;
};
