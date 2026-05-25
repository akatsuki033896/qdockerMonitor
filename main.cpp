#include "container.h"
#include "containerManager.h"
#include "mainwindow.h"
#include <QApplication>
#include <QtCore/qdebug.h>
#include <QtCore/qobject.h>
#include <QtCore/qobjectdefs.h>
#include <QtCore/qtimer.h>
#include <QTimer>
#include "util.h"
#include <QMessageBox>
#include <QFile>
#include <qthread.h>
#include <qthreadpool.h>


// docker ps --format "{{.ID}} {{.Names}}"
// docker stats --no-stream --format "{{.Container}} {{.CPUPerc}} {{.MemUsage}}"

// 待实现功能
// docker top <container_ID> 容器内部进程监控

ContainerManager manager;
QThreadPool* tp = QThreadPool::globalInstance();

void timer_task() {
    manager.refresh();
    auto list = manager.get_container_list();
    std::vector<std::string> ids;
    for (const auto& c : list) {
        ids.push_back(c.id);
    }
    for (const auto& id : ids) {
        tp->start(
            [id, mng = &manager]() {
                Container temp;
                temp.id = id;
                getStat(temp);
                getInspect(temp);
                QMetaObject::invokeMethod(mng, [mng, temp]() {
                    mng->update(temp);
                }, Qt::QueuedConnection);
            }
        );
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // set css
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString style = QLatin1String(file.readAll());
        app.setStyleSheet(style);
    }

    if (!isDockerRunning()) {
        qDebug() << "Docker is not running";
        QMessageBox::critical(nullptr, "Error", "Docker engine is not running!");
        return -1;
    }

    QTimer timer; // 定时器定时刷新
    QObject::connect(&timer, &QTimer::timeout, timer_task);
    timer.start(5000); // 5s 执行一次 事件驱动
    
    MainWindow win(&manager);
    QObject::connect(&manager, &ContainerManager::ContainerListChanged, &win, &MainWindow::onContainerListChanged);
    QObject::connect(&manager, &ContainerManager::ContainerStatsUpdated, &win, &MainWindow::onContainerStatsUpdated);
    win.show();
    return app.exec();
}
