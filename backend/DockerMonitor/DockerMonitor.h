//
// Created by akatsuki on 2026/6/7.
//

#ifndef SYSTEMMONITOR_DOCKERMONITOR_H
#define SYSTEMMONITOR_DOCKERMONITOR_H

#include <QtCore>
#include "../ContainerManager/containerManager.h"

class DockerMonitor : public QObject {
    Q_OBJECT
    QTimer *timer;
    QThreadPool *tp;
    ContainerManager m_manager;
    std::atomic_bool stopping = false;
public:
    ContainerManager* get_mng() { return &m_manager;  }
    explicit DockerMonitor(QObject *parent = nullptr);
    void start() { 
        timer->start(5000);
    }
    void stop() {
        stopping = true;
        timer->stop();
    }
    DockerMonitor(const DockerMonitor&) = delete;
    DockerMonitor(DockerMonitor&&) = delete;
    DockerMonitor operator=(const DockerMonitor&) = delete;
    DockerMonitor operator=(DockerMonitor&&) = delete;
    ~DockerMonitor();
private slots:
    void refresh(); // 刷新
};



#endif //SYSTEMMONITOR_DOCKERMONITOR_H
