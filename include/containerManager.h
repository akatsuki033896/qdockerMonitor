#pragma once
#include "container.h"
#include <QtCore/qobject.h>
#include <QtCore/qtmetamacros.h>
#include <vector>
#include <mutex>

class ContainerManager : public QObject {
    Q_OBJECT
    std::vector<Container> container_list; // 容器列表 如果使用vector，扩容之后 所有之前线程里的 &con 全部变成野指针
    std::mutex mtx;
public:
    ContainerManager();
    void refresh();
    void update(const Container& con);
    std::vector<Container> get_container_list();

    // 持有锁 无法拷贝
    ContainerManager(const ContainerManager&) = delete;
    ContainerManager operator=(const ContainerManager&) = delete;
signals:
    // void containerUpdated(const std::vector<Container>& list); // 只声明不实现
    void ContainerListChanged(const std::vector<Container>& list); // 更新列表（左边）
    void ContainerStatsUpdated(const Container& con); // 更新单个页面（右边）
};