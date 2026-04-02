#include "containerManager.h"
#include <sstream>
#include "util.h"
#include <mutex>

ContainerManager::ContainerManager() {
    refresh();
}

void ContainerManager::refresh() {
    std::vector<Container> newList;

    std::string list = getInfo("docker ps --format \"{{.ID}} {{.Names}}\""); // 获取容器ID列表
    // std::cout << list << '\n';
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
    emit containerUpdated(container_list); 
}

std::vector<Container> ContainerManager::get_container_list() {
    std::lock_guard<std::mutex> lock(mtx);
    return  container_list;
}

void ContainerManager::update(const Container& newCon) {
    {
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
    emit containerUpdated(container_list); // 更新UI
}