//
// Created by akatsuki on 2026/6/6.
//

#include "DataAcquireModule.h"
#include <qdebug.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <QRegularExpression>
#include <QEventLoop>
#include <QObject>
#include <QDebug>
#include <QNetworkReply>
#include "json.hpp"
#include "container.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

using json = nlohmann::json;

// request docker api from port 2375 via HTTP
QString DataAcquireModule::requestDockerAPI(const QString& method, const QString& endpoint) {
    QNetworkRequest request(QUrl("http://127.0.0.1:2375" + endpoint));
    QNetworkReply* reply = nullptr;
    if (method == "GET") {
        reply = manager.get(request);
    } else if (method == "POST") {
        reply = manager.post(request, QByteArray());
    } else {
        qDebug() << "[ERROR] unsupported method";
        return "";
    }

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "[ERROR] docker api error: " << reply->errorString();
        delete reply;
        return "";
    }
    QString result = QString::fromUtf8(reply->readAll());
    reply->deleteLater(); // 异步删除
    return result;
}

bool DataAcquireModule::getStat(Container& c) {
    if (c.id.empty()) {
        qDebug() << "get id failed.\n";
        return false;
    }

    QString output = requestDockerAPI("GET", "/containers/" + QString::fromStdString(c.id) + "/stats?stream=false");
    if (output.isEmpty()) {
        qDebug() << "stats empty for id: " << c.id;
        return false;
    }

    try {
        json j = json::parse(output.toStdString());
        // c.id = j["Container"]; // long id

        if (j.empty()) {
            qDebug() << "stat json empty for id: " << c.id;
            return false;
        }

        // container name
        c.name = j["name"];
        // qDebug() << "Name: " << c.name;

        // memory
        uint64_t usage = j["memory_stats"].value("usage", uint64_t(0));
        uint64_t limit = j["memory_stats"].value("limit", uint64_t(0));
        // qDebug() << usage << " " << limit;
        c.memPercent = limit > 0 ? usage * 100.0 / limit : 0.0;
        qDebug() << "mem:" << c.memPercent;

        // cpu
        uint64_t cpuTotal =
            j["cpu_stats"]["cpu_usage"].value("total_usage", uint64_t(0));
        uint64_t preCpuTotal =
            j["precpu_stats"]["cpu_usage"].value("total_usage", uint64_t(0));

        uint64_t systemTotal =
            j["cpu_stats"].value("system_cpu_usage", uint64_t(0));
        uint64_t preSystemTotal =
            j["precpu_stats"].value("system_cpu_usage", uint64_t(0));

        int onlineCpus = j["cpu_stats"].value("online_cpus", 1);

        uint64_t cpuDelta = cpuTotal - preCpuTotal;
        uint64_t systemDelta = systemTotal - preSystemTotal;

        if (cpuDelta > 0 && systemDelta > 0) {
            c.cpuPercent =
                (double)cpuDelta / (double)systemDelta * onlineCpus * 100.0;
        }
        else {
            c.cpuPercent = 0.0;
        }
        qDebug() << "cpu:" << c.cpuPercent;
        return true;
    }
    catch (const std::exception& e) {
        qDebug() << "[ERROR] stats parse error: " << e.what();
        qDebug().noquote() << output.left(5000);
        return false;
    }
    return true;
}

bool DataAcquireModule::getInspect(Container& c) {
    c.ports.clear();
    c.mounts.clear();

    QString output = requestDockerAPI(
        "GET", "/containers/" + QString::fromStdString(c.id) + "/json");

    std::string inspect_str = output.toStdString();
    if (inspect_str.empty()) {
        qDebug() << "inspect empty for id: " << c.id;
        return false;
    }

    // parse json
    json obj = json::parse(inspect_str);
    if (obj.empty()) {
        qDebug() << "inspect json empty for id: " << c.id;
        return false;
    }
    if (!obj.is_object()) {
        qDebug() << "inspect json is not object";
        return false;
    }

    if (obj.contains("Config") && obj["Config"].contains("Image")) {
        // c.image = obj["Config"]["Image"];
        c.image = obj["Config"].value("Image", "");
    }

    // 状态
    if (obj.contains("State")) {
        auto& state = obj["State"];
        if (state.contains("Status")) c.status = state.value("Status", "");
        if (state.contains("Running")) c.running = state["Running"];
        if (state.contains("StartedAt"))
            c.startedAt = state.value("StartedAt", "");
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
                if (value.is_array() && !value.empty() &&
                    value[0].contains("HostPort")) {
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
                c.mounts.emplace_back(m.value("Source", ""), m.value("Destination", ""));
                // FIXME: push_back -> emplace_back
            }
        }
    }
    // print container information
    return true;
}

// 获取进程
bool DataAcquireModule::getProcess(Container& c) {
    if (c.id.empty()) {
        qDebug() << "get id failed";
        return false;
    }
    QString output = requestDockerAPI("GET", "/containers/" + QString::fromStdString(c.id) + "/top");
    QJsonDocument doc = QJsonDocument::fromJson(output.toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "Invalid Process JSON format";
        return false;
    }
    QJsonObject root_obj = doc.object();
    if (root_obj.contains("Titles")) {
        QJsonArray title = root_obj["Titles"].toArray();
    }
    if (root_obj.contains("Processes")) {
        QJsonArray ps = root_obj["Processes"].toArray();
        c.processes.clear();
        for (const auto& value: ps) {
            auto row = value.toArray();
            ProcessInfo p;
            p.uid = row[0].toString();
            p.pid = row[1].toString();
            p.ppid = row[2].toString();
            p.cpu = row[3].toString();
            p.stime = row[4].toString();
            p.tty = row[5].toString();
            p.time = row[6].toString();
            p.cmd = row[7].toString();
            c.processes.push_back(std::move(p));
        }
        // qDebug() << ps[0];
    }
    return true;
}
