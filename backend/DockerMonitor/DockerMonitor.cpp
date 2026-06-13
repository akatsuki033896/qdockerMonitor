#include "DockerMonitor.h"
#include <qthreadpool.h>
#include <QObject>
#include <QDebug>
#include <QTimer>
#include "../DockerClient/DataAcquireModule/DataAcquireModule.h"
#include <chrono>
#include <mutex>

DockerMonitor::DockerMonitor(QObject *parent) {
    timer = new QTimer(this);
    tp = new QThreadPool(this);
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
                Container con;
                DataAcquireModule dm;
                con.id = id;

                // 获取容器状态
                auto now = std::chrono::steady_clock::now();
                bool need_inspect = false;

                if (dm.getStat(con)) {
                    std::lock_guard<std::mutex> lock(mng->state_mtx);
                    mng->state_map[id].lastStatsTime = now;
                }
                // 获取容器信息
                {
                    std::lock_guard<std::mutex> lock(mng->state_mtx);
                    auto& state = mng->state_map[id];
                    auto interval = std::chrono::duration_cast<std::chrono::seconds>(
                        now - state.lastInspectTime);
                    if (interval.count() >= 10) {
                        state.lastInspectTime = now;
                        need_inspect = true;
                    }
                }
                if (need_inspect) {
                    dm.getInspect(con);
                }
                dm.getProcess(con);
                QMetaObject::invokeMethod(mng, [mng, con]() {
                    mng->update(con);
                }, Qt::QueuedConnection);
            }
        );
    }
}

DockerMonitor::~DockerMonitor() {
    stop();
    tp->waitForDone();
} // FIXME:没有正确析构