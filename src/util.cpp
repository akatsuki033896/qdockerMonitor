#include "util.h"
#include <iostream>
#include "../include/json.hpp"
#include <QProcess>
#include <qcontainerfwd.h>
#include <QDebug>
#include <qdebug.h>
#include <qprocess.h>
#include <string>

using json = nlohmann::json;

// std::string getInfo(std::string str) {
//     std::string result;
//     char buffer[128];
//     FILE* fp = _popen(str.c_str(), "r");
//     if (!fp) {
//         std::cout << "popen failed" << '\n';
//         exit(EXIT_FAILURE);
//     }
//     while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
//         result += buffer;
//     }
//     // std::cout << result << '\n';
//     _pclose(fp);
//     return result;
// }

QString runDocker(const QStringList& args) {
    QProcess process;
    process.start("docker", args);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    QString error = process.readAllStandardError();

    if (!error.isEmpty()) {
        qDebug() << "Docker command error:" << error;
        return "";
    }
    return output;
}

bool getStat(Container& c) {
    if (c.id.empty()) {
        std::cout << "get id failed.\n";
        return false;
    }

    QStringList args;
    args << "stats" << QString::fromStdString(c.id) << "--no-stream" << "--format" << "{{json .}}";
    QString output = runDocker(args).trimmed();
    std::string stats_str = output.toStdString();
    
    if (stats_str.empty()) {
        std::cout << "stats empty for id: " << c.id << '\n';
        return false;
    }

    json j = json::parse(stats_str);
    // c.id = j["Container"]; // long id

    if (j.empty()) {
        std::cout << "inspect json empty for id: " << c.id << '\n';
        return false;
    }

    c.name = j["Name"];
    std::string cpu = j["CPUPerc"];
    c.cpuPercent = std::stod(cpu.substr(0, cpu.size() - 1));
    std::string mem = j["MemPerc"];
    c.memPercent = std::stod(mem.substr(0, mem.size() - 1));
    c.memUsage = j["MemUsage"];
    return true;
}

// 传引用。不然改不了结构体的值。
bool getInspect(Container& c) {
    c.ports.clear();
    c.mounts.clear();

    QString output = runDocker({"inspect", QString::fromStdString(c.id)}).trimmed();
    
    std::string inspect_str = output.toStdString();
    if (inspect_str.empty()) {
        std::cout << "inspect empty for id: " << c.id << '\n';
        return false;
    }
    
    // qDebug().noquote() << inspect_str;
    // qDebug().noquote() << QString::fromStdString(inspect_str);
    
    // parse json 
    json j = json::parse(inspect_str);
    
    if (j.empty()) {
        std::cout << "inspect json empty for id: " << c.id << '\n';
        return false;
    }
    else if (!j.is_array()) {
        std::cout << "inspect json is not an array for id: " << c.id << '\n';
        return false;
    }
    
    json obj = j[0];
    if (obj.contains("Config") && obj["Config"].contains("Image")) {
        c.image = obj["Config"]["Image"];
        qDebug() << "Image: " << c.image;
    }
    
    // 状态
    if (obj.contains("State")) {
        auto& state = obj["State"];
        if (state.contains("Status")) c.status = state["Status"];
        if (state.contains("Running")) c.running = state["Running"];
        if (state.contains("StartedAt")) c.startedAt = state["StartedAt"];
        qDebug() << "Status: " << c.status;
        qDebug() << "Running: " << c.running;
        qDebug() << "StartedAt: " << c.startedAt;
    }
    
    // 网络
    if (obj.contains("NetworkSettings")) {
        auto& net = obj["NetworkSettings"];
        if (net.contains("IPAddress")) {
            c.ip = net["IPAddress"];
            c.mac = net["Networks"]["bridge"]["MacAddress"];
        }
        // 桥接网络
        else if (net.contains("Networks")) {
            c.ip = net["Networks"]["bridge"]["IPAddress"];
            c.mac = net["Networks"]["bridge"]["MacAddress"];
        }
        if (net.contains("Ports")) {
            for (auto& [container_port, value] : net["Ports"].items()) {
                if (value.is_array() && !value.empty() && value[0].contains("HostPort")) {
                    c.ports[container_port] = value[0]["HostPort"];
                }
            }
        }
        qDebug() << "IP: " << c.ip;
        qDebug() << "MAC: " << c.mac;
        qDebug() << "Ports: ";
        for (const auto& [container_port, host_port] : c.ports) {
            qDebug() << "  " << container_port.c_str() << " -> " << host_port.c_str();
        }
    }
    
    // 挂载
    if (obj.contains("Mounts")) {
        for (auto& m : obj["Mounts"]) {
            if (m.contains("Source") && m.contains("Destination")) {
                c.mounts.push_back({m["Source"], m["Destination"]});
            }
        }
        qDebug() << "Mounts: ";
        for (const auto& mount : c.mounts) {
            qDebug() << "  " << mount.first.c_str() << " -> " << mount.second.c_str();
        }   
    }
    return true;
}
