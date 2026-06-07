//
// Created by akatsuki on 2026/6/7.
//

#include "DockerMonitor.h"
#include <QObject>
#include <QDebug>
#include "util.h"
#include <chrono>

DockerMonitor::DockerMonitor(QObject *parent) {
    timer = new QTimer(this);
    tp = QThreadPool::globalInstance();
    QObject::connect(timer, &QTimer::timeout, this, &DockerMonitor::refresh);
}

void DockerMonitor::refresh() {
    m_manager.refresh();
    auto list = m_manager.get_container_list();
    std::vector<std::string> ids;
    for (const auto& c : list) {
        ids.push_back(c.id);
    }
    for (const auto& id : ids) {
        tp->start(
            [id, mng = &m_manager]() {
                Container temp;
                temp.id = id;
                // 获取容器状态
                auto state = mng->state_map[id];
                auto now = std::chrono::steady_clock::now();
                if (getStat(temp)) {
                    state.lastStatsTime = now;
                }
                // 获取容器信息
                auto inspectInterval = std::chrono::duration_cast<std::chrono::seconds>(now - state.lastInspectTime);
                if (inspectInterval.count() >= 10) {
                    getInspect(temp);
                    state.lastInspectTime = now;
                }
                // getProcess(temp);
                QMetaObject::invokeMethod(mng, [mng, temp]() {
                    mng->update(temp);
                }, Qt::QueuedConnection);
            }
        );
    }
}