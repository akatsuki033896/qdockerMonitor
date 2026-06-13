#include "backend/ContainerManager/containerManager.h"
#include "mainwindow.h"
#include <QApplication>
#include <QtCore>
#include "util.h"
#include <QMessageBox>
#include <QFile>
#include "backend/DockerMonitor/DockerMonitor.h"

// TODO: button
// TODO: File system

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

    DockerMonitor monitor;
    auto mng = monitor.get_mng();
    MainWindow win(mng);
    QObject::connect(mng, &ContainerManager::ContainerListChanged, &win, &MainWindow::onContainerListChanged);
    QObject::connect(mng, &ContainerManager::ContainerStatsUpdated, &win, &MainWindow::onContainerStatsUpdated);
    monitor.start();
    win.show();
    return app.exec();
}
