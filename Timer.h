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

    void ElapsedTime() {
        std::cout << "Elapsed Time : " << std::fixed << std::chrono::duration<double>(endAt - startAt).count() << "s" << std::endl;
    }
private:
    typedef std::chrono::time_point<std::chrono::steady_clock> clock_point;
    clock_point startAt, endAt;

    clock_point now() {
        return std::chrono::steady_clock::now();
    }
};