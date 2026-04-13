#pragma once

#include "imgui.h"
#include "ics_tab.hpp"
#include <vector>
#include <string>
#include <map>

namespace Tabs {

struct ArduinoPin {
    static constexpr size_t MAX_WAVEFORM_SIZE = 200;
    
    int pinNumber;
    std::string mode;
    float value;
    bool isDigital;
    std::vector<float> waveformData;
    
    ArduinoPin() : pinNumber(0), mode("INPUT"), value(0.0f), isDigital(true) {
        waveformData.reserve(MAX_WAVEFORM_SIZE);
    }
};

struct WavePlotter {
    static constexpr int MAX_SAMPLES_LIMIT = 2000;
    static constexpr size_t MAX_CHANNELS = 16;
    
    std::string name;
    std::vector<std::vector<float>> channels;
    int maxSamples;
    bool is3D;
    bool paused;
    
    WavePlotter() : maxSamples(1000), is3D(false), paused(false) {
        channels.reserve(MAX_CHANNELS);
    }
};

class IKSTab {
public:
    IKSTab();
    ~IKSTab() = default;

    void render();
    const char* getName() const { return "IKS"; }
    const char* getIcon() const { return ICON_IKS; }
    
    void setICSReference(ICSTab* ics) { icsRef = ics; }
    void renderCodeEditor();
    void renderCircuitDesigner();
    void renderWavePlotter();
    void renderPinController();
    
    void compileCode();
    void uploadToArduino();
    void addWaveChannel();
    void linkToICS();

private:
    static constexpr const char* ICON_IKS = "IKS";
    
    ICSTab* icsRef;
    std::map<int, ArduinoPin> pins;
    std::vector<WavePlotter> plotters;
    
    std::string arduinoCode;
    std::string compileOutput;
    bool isCompiling;
    bool showCircuitDesigner;
    bool showWavePlotter;
    int totalPins;
    bool infinitePinMode;
    
    char codeBuffer[8192];
    
    void initializePins();
    void updateWaveforms();
    void render2DWave(const WavePlotter& plotter);
    void render3DWave(const WavePlotter& plotter);
};

}
