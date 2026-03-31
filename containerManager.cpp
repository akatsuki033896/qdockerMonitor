#include "containerManager.h"
#include <sstream>
#include "util.h"
#include <mutex>

ContainerManager::ContainerManager() {
    refresh();
}

void ContainerManager::refresh() {
    std::string list = getInfo("docker ps --format \"{{.ID}} {{.Names}}\""); // 获取容器ID列表
    // std::cout << list << '\n';
    std::istringstream iss(list);
    std::string id, name;
    while (iss >> id >> name) {
        Container c;
        c.id = id;
        c.name = name;
        container_list.push_back(std::move(c));
    }
}

std::vector<Container> ContainerManager::get_container_list() {
    std::lock_guard<std::mutex> lock(mtx);
    return  container_list;
}

void ContainerManager::update(Container& newCon) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& con : container_list) {
        if (con.id == newCon.id) {
            con = newCon;
            return;
        }
    }
    // 写回 之后改成哈希表
    // do some update
}