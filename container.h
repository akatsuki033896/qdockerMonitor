#pragma once
#include <string>
#include <map>
#include <vector>

// 存储container 数据，若持有锁则违反单一职责原则
struct Container {
    std::string id;
    std::string name;
    std::string image;

    std::string status;
    bool running;
    std::string startedAt;

    std::string ip;
    std::map<std::string, std::string> ports;

    // 挂载地址 pair<src, dst>
    std::vector<std::pair<std::string, std::string>> mounts;

    double cpuPercent = 0.0;
    double memPercent = 0.0;
    std::string memUsage;

    Container() = default;
};