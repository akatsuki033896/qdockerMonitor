#pragma once
#include "container.h"
#include <vector>
#include <mutex>

class ContainerManager {
    std::vector<Container> container_list; // 容器列表 如果使用vector，扩容之后 所有之前线程里的 &con 全部变成野指针
    std::mutex mtx;
public:
    ContainerManager();
    void refresh();
    void update(Container& con);
    
    std::vector<Container> get_container_list();

    // 持有锁 无法拷贝
    ContainerManager(const ContainerManager&) = delete;
    ContainerManager operator=(const ContainerManager&) = delete;
};