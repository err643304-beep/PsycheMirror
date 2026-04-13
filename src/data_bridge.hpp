#pragma once

#include <vector>
#include <mutex>
#include <functional>
#include <map>
#include <string>

struct SubPixel {
    float r, g, b, a;
    float x, y;
    int row, col;
    std::string equation;
    bool selected;
    bool active;
    
    SubPixel() : r(0), g(0), b(0), a(1), x(0), y(0), row(0), col(0), selected(false), active(true) {}
};

class DataBridge {
public:
    static DataBridge& getInstance() {
        static DataBridge instance;
        return instance;
    }

    struct ArduinoToPixelMapping {
        int pinNumber;
        std::vector<int> pixelIndices;
        std::function<float(float)> transformFunc;
        bool enabled;
        
        ArduinoToPixelMapping() : pinNumber(0), enabled(true) {
            transformFunc = [](float v) { return v; };
        }
    };
    
    struct PixelToArduinoMapping {
        int pixelIndex;
        int pinNumber;
        char channel;
        bool enabled;
        
        PixelToArduinoMapping() : pixelIndex(0), pinNumber(0), channel('r'), enabled(true) {}
    };
    
    void setPixelData(std::vector<SubPixel>* pixels) {
        std::lock_guard<std::mutex> lock(dataMutex);
        pixelDataRef = pixels;
    }
    
    std::vector<SubPixel>* getPixelData() {
        std::lock_guard<std::mutex> lock(dataMutex);
        return pixelDataRef;
    }
    
    void setArduinoPinValue(int pin, float value) {
        std::lock_guard<std::mutex> lock(dataMutex);
        arduinoPinValues[pin] = value;
        updatePixelsFromArduino(pin, value);
    }
    
    float getArduinoPinValue(int pin) {
        std::lock_guard<std::mutex> lock(dataMutex);
        return arduinoPinValues[pin];
    }
    
    void setPixelToArduino(int pixelIndex, int pin, char channel) {
        std::lock_guard<std::mutex> lock(dataMutex);
        PixelToArduinoMapping mapping;
        mapping.pixelIndex = pixelIndex;
        mapping.pinNumber = pin;
        mapping.channel = channel;
        pixelToArduino.push_back(mapping);
    }
    
    void addArduinoToPixelMapping(int pin, const std::vector<int>& pixels) {
        std::lock_guard<std::mutex> lock(dataMutex);
        ArduinoToPixelMapping mapping;
        mapping.pinNumber = pin;
        mapping.pixelIndices = pixels;
        arduinoToPixel[pin] = mapping;
    }
    
    void updateArduinoFromPixels();
    void updatePixelsFromArduino(int pin, float value);
    
    void broadcastEquation(const std::string& equation) {
        std::lock_guard<std::mutex> lock(dataMutex);
        sharedEquation = equation;
    }
    
    std::string getSharedEquation() {
        std::lock_guard<std::mutex> lock(dataMutex);
        return sharedEquation;
    }
    
    std::mutex& getMutex() { return dataMutex; }
    
    std::vector<ArduinoToPixelMapping>& getArduinoMappings() {
        return arduinoToPixelVector;
    }
    
    std::vector<PixelToArduinoMapping>& getPixelMappings() {
        return pixelToArduino;
    }
    
    void clearAllMappings() {
        std::lock_guard<std::mutex> lock(dataMutex);
        arduinoToPixel.clear();
        arduinoToPixelVector.clear();
        pixelToArduino.clear();
        arduinoPinValues.clear();
    }
    
    void clearArduinoMapping(int pin) {
        std::lock_guard<std::mutex> lock(dataMutex);
        arduinoToPixel.erase(pin);
        arduinoPinValues.erase(pin);
    }
    
    void clearPixelMapping(int pixelIndex) {
        std::lock_guard<std::mutex> lock(dataMutex);
        pixelToArduino.erase(
            std::remove_if(pixelToArduino.begin(), pixelToArduino.end(),
                [pixelIndex](const PixelToArduinoMapping& m) { return m.pixelIndex == pixelIndex; }),
            pixelToArduino.end()
        );
    }
    
    void limitMappingSizes() {
        std::lock_guard<std::mutex> lock(dataMutex);
        const size_t MAX_MAPPINGS = 1024;
        if (pixelToArduino.size() > MAX_MAPPINGS) {
            pixelToArduino.resize(MAX_MAPPINGS);
        }
        if (arduinoToPixelVector.size() > MAX_MAPPINGS) {
            arduinoToPixelVector.resize(MAX_MAPPINGS);
        }
    }

private:
    DataBridge() : pixelDataRef(nullptr) {}
    ~DataBridge() = default;
    DataBridge(const DataBridge&) = delete;
    DataBridge& operator=(const DataBridge&) = delete;
    
    std::mutex dataMutex;
    std::vector<SubPixel>* pixelDataRef;
    std::map<int, float> arduinoPinValues;
    std::map<int, ArduinoToPixelMapping> arduinoToPixel;
    std::vector<ArduinoToPixelMapping> arduinoToPixelVector;
    std::vector<PixelToArduinoMapping> pixelToArduino;
    std::string sharedEquation;
};
