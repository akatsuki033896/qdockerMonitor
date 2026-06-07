#pragma once
#include <chrono>

struct ContainerState {
    std::chrono::steady_clock::time_point lastStatsTime;
    std::chrono::steady_clock::time_point lastInspectTime;
    bool inspect_refresh = false;
};