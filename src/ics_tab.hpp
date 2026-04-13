#pragma once

#include "imgui.h"
#include "data_bridge.hpp"
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

namespace Tabs {

class ICSTab {
public:
    ICSTab();
    ~ICSTab();

    void render();
    const char* getName() const { return "ICS"; }
    const char* getIcon() const { return ICON_ICS; }
    
    void renderPixelGrid();
    void renderPixelData();
    void handlePixelSelection();
    void exportSelectedToECI();
    std::vector<SubPixel>& getPixels() { return pixels; }
    std::mutex& getPixelMutex() { return pixelMutex; }

private:
    static constexpr const char* ICON_ICS = "ICS";
    static constexpr int GRID_WIDTH = 64;
    static constexpr int GRID_HEIGHT = 64;
    static constexpr float PIXEL_SIZE = 3.0f;
    
    std::vector<SubPixel> pixels;
    std::vector<int> selectedPixels;
    std::mutex pixelMutex;
    std::atomic<bool> threadRunning;
    std::thread renderThread;
    
    float zoom;
    ImVec2 scrollOffset;
    bool showGrid;
    bool showCoordinates;
    
    void initializePixels();
    void updatePixelFromEquation(SubPixel& pixel, float time);
    void startRenderThread();
    void stopRenderThread();
};

}
