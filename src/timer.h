#pragma once
#include <chrono>

// Simple timer class for milliseconds measurement
class Timer {
    std::chrono::high_resolution_clock::time_point start_time;
public:
    void start() { start_time = std::chrono::high_resolution_clock::now(); }
    // Returns elapsed time in milliseconds since start()
    float stop() {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> duration = end_time - start_time;
        return duration.count();
    }
};
