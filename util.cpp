#include "util.h"
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

std::string getInfo(std::string str) {
    std::string result;
    char buffer[128];
    FILE* fp = popen(str.c_str(), "r");
    if (!fp) {
        std::cout << "popen failed" << '\n';
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        result += buffer;
    }
    // std::cout << result << '\n';
    pclose(fp);
    return result;
}

void getStat(Container& c) {
    // stat
    if (c.id.empty()) {
        std::cout << "get id failed.\n";
    }
    // std::cout << "start get stat" << '\n';
    std::string cmd = "docker stats " + c.id + " --no-stream --format \"{{json .}}\"";
    std::string stats_str = getInfo(cmd);
    if (stats_str.empty()) {
        std::cout << "stats empty for id: " << c.id << '\n';
        return;
    }

    json j = json::parse(stats_str);
    // c.id = j["Container"]; // long id
    c.name = j["Name"];
    std::string cpu = j["CPUPerc"];
    c.cpuPercent = std::stod(cpu.substr(0, cpu.size() - 1));
    std::string mem = j["MemPerc"];
    c.memPercent = std::stod(mem.substr(0, mem.size() - 1));
    c.memUsage = j["MemUsage"];
}

// 传引用。不然改不了结构体的值。
void getInspect(Container& c) {
    c.ports.clear();
    c.mounts.clear();

    // std::cout << "Start get inspect" << '\n';
    // inspect
    std::string inspect_str = getInfo("docker inspect " + c.id);
    if (inspect_str.empty()) {
        std::cout << "stats empty for id: " << c.id << '\n';
        return;
    }
    json j = json::parse(inspect_str);
    json obj = j[0];
    c.image = obj["Config"]["Image"];

    c.status = obj["State"]["Status"];
    c.running = obj["State"]["Running"];
    c.startedAt = obj["State"]["StartedAt"];
    c.ip = obj["NetworkSettings"]["IPAddress"];
    auto ports = obj["NetworkSettings"]["Ports"];

    // 端口映射
    for (auto& [container_port, value] : ports.items()) {
        if (!value.is_null()) {
            std::string host_port = value[0]["HostPort"];
            c.ports[container_port] = host_port;
            std::cout << c.ports[container_port] << '\n';
        }
    }

    // 挂载
    for (auto& m : obj["Mounts"]) {
        std::string src = m["Source"];
        std::string dst = m["Destination"];
        c.mounts.push_back({src, dst});
    }

    std::cout << "Name: " << c.name << '\n';
    std::cout << "CPU: " << c.cpuPercent << "%\n";
    std::cout << "Mem: " << c.memUsage << '\n';
    std::cout << "IP: " << c.ip << '\n';
    std::cout << "Image: " << c.image << '\n';
}
