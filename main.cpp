#include "container.h"
#include "containerManager.h"
#include "mainwindow.h"
#include <QApplication>
#include <QtCore/qdebug.h>
#include <QtCore/qobject.h>
#include <QtCore/qobjectdefs.h>
#include <QtCore/qtimer.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include "threadpool.hpp"
#include <QTimer>
#include "util.h"

// docker ps --format "{{.ID}} {{.Names}}"
// docker stats --no-stream --format "{{.Container}} {{.CPUPerc}} {{.MemUsage}}"

// 待实现功能
// docker top <container_ID> 容器内部进程监控

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    std::cout << "system monitor start running." << '\n';

    ContainerManager manager;
    ThreadPool pool;
    QTimer timer;

    // 定时器定时刷新
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        manager.refresh();

        auto list = manager.get_container_list();

        std::vector<std::string> ids;
        for (const auto& c : list) {
            ids.push_back(c.id);
        }

        for (const auto& id : ids) {
            pool.addTask([id, mgr = &manager]() {
                Container temp;
                temp.id = id;
                getStat(temp);
                getInspect(temp);
                // qDebug() << "before invoke";
                QMetaObject::invokeMethod(mgr, [mgr, temp]() {
                    // qDebug() << "invoke";
                    mgr->update(temp);
                }, Qt::QueuedConnection);
            });
        }
    });

    timer.start(3000); // 3s 执行一次 事件驱动

    // GUI

    MainWindow win;
    // 连接 UI
     
    QObject::connect(&manager, &ContainerManager::ContainerListChanged, &win, &MainWindow::onContainerListChanged);
    QObject::connect(&manager, &ContainerManager::ContainerStatsUpdated, &win, &MainWindow::onContainerStatsUpdated);
    win.show();
    return app.exec();
}
