#pragma once
#include <chrono>

struct State {
    std::chrono::steady_clock::time_point lastStatsTime;
    std::chrono::steady_clock::time_point lastInspectTime;
    bool inspect_refresh = false;
};