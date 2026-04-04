#include "containerManager.h"
#include <QtCore/qlogging.h>
#include <sstream>
#include "util.h"
#include <mutex>
#include <qdebug.h>
#include <QStringList>
#include <QProcess>

ContainerManager::ContainerManager() {
    refresh();
}

void ContainerManager::refresh() {
    std::vector<Container> newList;

    QStringList args;
    args << "ps" << "--format" << "{{.ID}} {{.Names}}";
    QString output = runDocker(args);
    std::string list = output.toStdString();
    std::istringstream iss(list);
    std::string id, name;
    while (iss >> id >> name) {
        Container c;
        c.id = id;
        c.name = name;
        newList.push_back(std::move(c));
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        container_list = std::move(newList);
    }
    emit ContainerListChanged(container_list);
}

std::vector<Container> ContainerManager::get_container_list() {
    std::lock_guard<std::mutex> lock(mtx);
    return  container_list;
}

void ContainerManager::update(const Container& newCon) {
    bool flag = false;
    {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& con : container_list) {
            if (con.id == newCon.id) {
                con = newCon;
                flag = true;
            }
        }
        // 写回 之后改成哈希表
        // do some update
    }
    if (flag) {
        emit ContainerStatsUpdated(newCon); // 更新UI
        // qDebug() << "emit statsUpdated";
    }
}