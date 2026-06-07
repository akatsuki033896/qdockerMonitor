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
public:
    ContainerManager* get_mng() { return &m_manager;  }
    explicit DockerMonitor(QObject *parent = nullptr);
    void start() { timer->start(5000); }
    void stop();
private slots:
    void refresh();
};



#endif //SYSTEMMONITOR_DOCKERMONITOR_H
