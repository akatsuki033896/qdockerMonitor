#include "container.h"
#include "containerManager.h"
#include "mainwindow.h"
#include <QApplication>
#include <QtCore/qobject.h>
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
    QApplication a(argc, argv);
    std::cout << "system monitor start running." << '\n';

    ContainerManager manager;
    ThreadPool pool;
    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [&]() {
        manager.refresh();
        auto list = manager.get_container_list();
        // std::cout << list[0].id << '\n';
        for (auto& con : list) {
            pool.addTask([&manager, id = con.id] {
                Container temp;
                temp.id = id;
                getStat(temp);
                // std::cout << "get stat done.\n";
                getInspect(temp);
                // std::cout << "get inspect done.\n";
                manager.update(temp);
            });
        }
    });

    timer.start(3000); // 3s 执行一次 事件驱动
    
    // GUI

    // MainWindow w;
    // w.show();
    return a.exec();
}
