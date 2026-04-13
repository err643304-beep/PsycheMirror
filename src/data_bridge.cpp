#include "data_bridge.hpp"
#include "ics_tab.hpp"
#include <cmath>
#include <algorithm>

void DataBridge::updateArduinoFromPixels() {
    try {
        if (!pixelDataRef) return;
        
        for (auto& mapping : pixelToArduino) {
            if (!mapping.enabled) continue;
            if (mapping.pixelIndex < 0 || mapping.pixelIndex >= pixelDataRef->size()) continue;
            
            const SubPixel& pixel = (*pixelDataRef)[mapping.pixelIndex];
            float value = 0.0f;
            
            switch (mapping.channel) {
                case 'r': value = pixel.r; break;
                case 'g': value = pixel.g; break;
                case 'b': value = pixel.b; break;
                case 'a': value = pixel.a; break;
                default: value = (pixel.r + pixel.g + pixel.b) / 3.0f; break;
            }
            
            arduinoPinValues[mapping.pinNumber] = value;
        }
    } catch (const std::exception& e) {
    } catch (...) {
    }
}

void DataBridge::updatePixelsFromArduino(int pin, float value) {
    try {
        if (!pixelDataRef) return;
        
        auto it = arduinoToPixel.find(pin);
        if (it == arduinoToPixel.end()) return;
        
        ArduinoToPixelMapping& mapping = it->second;
        if (!mapping.enabled) return;
        
        float transformedValue = mapping.transformFunc(value);
        transformedValue = std::max(0.0f, std::min(1.0f, transformedValue));
        
        for (int pixelIdx : mapping.pixelIndices) {
            if (pixelIdx >= 0 && pixelIdx < pixelDataRef->size()) {
                SubPixel& pixel = (*pixelDataRef)[pixelIdx];
                pixel.r = pixel.g = pixel.b = transformedValue;
                pixel.a = 1.0f;
            }
        }
    } catch (const std::exception& e) {
    } catch (...) {
    }
}
