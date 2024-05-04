#pragma once
#include <iostream>
#include <chrono>
#include <iomanip>

class Timer {
public:
    void start() {
        startAt = now();
    }
    
    void end() {
        endAt = now();
    }

    double timeSpend() {
        return std::chrono::duration<double>(endAt - startAt).count();
    }
private:
    typedef std::chrono::time_point<std::chrono::steady_clock> clock_point;
    clock_point startAt, endAt;

    clock_point now() {
        return std::chrono::steady_clock::now();
    }
};