#include "util.h"

#include <QDebug>
#include <QProcess>
#include <iostream>
#include <qregularExpression>
#include <string>
#include <qcoreevent.h>
#include <qnetworkrequest.h>
#include <QString>
#include "json.hpp"

using json = nlohmann::json;

bool isDockerRunning() {
    QProcess process;
    process.start("docker", {"ps"});
    if (!process.waitForFinished(3000)) {
        return false;
    } else if (process.exitStatus() != QProcess::NormalExit ||
               process.exitCode() != 0) {
        qDebug() << "Docker running process exit: " << process.exitCode();
        return false;
    }
    return true;
}

QString runDockerProcess(const QStringList& args) {
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

QString runDockerCLI(const QStringList& args) {
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

// TODO: 移动到日志模块
void printInfo(const Container& c) {
    qDebug() << "Image: " << c.image;
    qDebug() << "Status: " << c.status;
    qDebug() << "Running: " << c.running;
    qDebug() << "StartedAt: " << c.startedAt;
    qDebug() << "IP: " << c.ip;
    qDebug() << "MAC: " << c.mac;
    qDebug() << "Ports: ";
    for (const auto& [container_port, host_port] : c.ports) {
        qDebug() << "  " << container_port.c_str() << " -> "
                 << host_port.c_str();
    }
    qDebug() << "Mounts: ";
    for (const auto& mount : c.mounts) {
        qDebug() << "  " << mount.first.c_str() << " -> "
                 << mount.second.c_str();
    }
}