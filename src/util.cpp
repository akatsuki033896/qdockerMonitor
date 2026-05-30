#include "util.h"
#include <iostream>
#include "../include/json.hpp"
#include <QProcess>
#include <QDebug>
#include <qdebug.h>
#include <qprocess.h>
#include <string>
#include <qregularExpression>

using json = nlohmann::json;

bool isDockerRunning() {
    QProcess process;
    process.start("docker", {"ps"});
    if (!process.waitForFinished(3000)) {
        return false;
    }
    else if (process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0) {
        qDebug() << "Docker running process exit: " << process.exitCode();
        return false;
    }
    return true;
}

QString runDockerAPI(const QStringList& args) {
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
    QString output = runDockerAPI(args).trimmed();
    std::string stats_str = output.toStdString();
    
    if (stats_str.empty()) {
        qDebug() << "stats empty for id: " << c.id;
        return false;
    }

    json j = json::parse(stats_str);
    // c.id = j["Container"]; // long id

    if (j.empty()) {
        qDebug() << "inspect json empty for id: " << c.id;
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
    QString output = runDockerAPI({"inspect", QString::fromStdString(c.id)}).trimmed();
    std::string inspect_str = output.toStdString();
    if (inspect_str.empty()) {
        qDebug() << "inspect empty for id: " << c.id;
        return false;
    }
    
    // parse json 
    json j = json::parse(inspect_str);
    if (j.empty()) {
        qDebug() << "inspect json empty for id: " << c.id;
        return false;
    }
    else if (!j.is_array()) {
        qDebug() << "inspect json is not an array for id: " << c.id;
        return false;
    }
    
    json obj = j[0];
    if (obj.contains("Config") && obj["Config"].contains("Image")) {
        // c.image = obj["Config"]["Image"];
        c.image = obj["Config"].value("Image", "");
    }

    // 状态
    if (obj.contains("State")) {
        auto& state = obj["State"];
        if (state.contains("Status")) c.status = state.value("Status", "");
        if (state.contains("Running")) c.running = state["Running"];
        if (state.contains("StartedAt")) c.startedAt = state.value("StartedAt", "");
    }
    
    // 部分容器没有挂载地址
    if (obj.contains("NetworkSettings")) {
        auto& net = obj["NetworkSettings"];
        if (net.contains("IPAddress") && !net["IPAddress"].is_null()) {
            c.ip = net["IPAddress"];
            c.mac = net["Networks"]["bridge"]["MacAddress"];
        }
        // 桥接网络
        else if (!net["Network"]["bridge"].is_null()) {
            c.ip = net["Networks"]["bridge"]["IPAddress"];
            c.mac = net["Networks"]["bridge"]["MacAddress"];
        }
        if (net.contains("Ports")) {
            for (auto& [container_port, value] : net["Ports"].items()) {
                if (value.is_array() && !value.empty() && value[0].contains("HostPort")) {
                    // c.ports[container_port] = value[0]["HostPort"];
                    c.ports[container_port] = value[0].value("HostPort", "");
                }
            }
        }
    }
    
    // 挂载
    if (obj.contains("Mounts")) {
        for (auto& m : obj["Mounts"]) {
            if (m.contains("Source") && m.contains("Destination")) {
                // c.mounts.push_back({m["Source"], m["Destination"]});
                c.mounts.push_back({m.value("Source", ""), m.value("Destination", "")});
            }
        }
    }
    // print container information
    return true;
}

void printInfo(const Container& c)
{
    qDebug() << "Image: " << c.image;
    qDebug() << "Status: " << c.status;
    qDebug() << "Running: " << c.running;
    qDebug() << "StartedAt: " << c.startedAt;
    qDebug() << "IP: " << c.ip;
    qDebug() << "MAC: " << c.mac;
    qDebug() << "Ports: ";
    for (const auto& [container_port, host_port] : c.ports) {
        qDebug() << "  " << container_port.c_str() << " -> " << host_port.c_str();
    }
    qDebug() << "Mounts: ";
    for (const auto& mount : c.mounts) {
        qDebug() << "  " << mount.first.c_str() << " -> " << mount.second.c_str();
    }
}

bool getProcess(const Container& c)
{
    if (c.id.empty()) {
        qDebug() << "get id failed";
        return false;
    }
    QString output = runDockerAPI({"top", QString::fromStdString(c.id)}).trimmed();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    for (int i = 1; i < lines.size(); i++)
    {
        QStringList cols = lines[i].split(QRegularExpression("\\s+"),
                   Qt::SkipEmptyParts);
        qDebug() << cols.join(" ") << "\n";
    }
    return true;
}
