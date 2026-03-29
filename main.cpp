#include "mainwindow.h"
#include <QApplication>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstdio>
#include "json.hpp"

using json = nlohmann::json;

// docker ps --format "{{.ID}} {{.Names}}"
// docker stats --no-stream --format "{{.Container}} {{.CPUPerc}} {{.MemUsage}}"

std::string getInfo(std::string str) {
    std::string result;
    char buffer[128];
    FILE* fp = popen(str.c_str(), "r");
    if (!fp) {
        std::cout << "popen failed" << '\n';
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        result += buffer;
    }
    // std::cout << result << '\n';
    pclose(fp);
    return result;
}

int main(int argc, char *argv[]) {
    std::cout << "system monitor start running." << '\n';
 
    std::string info_str = getInfo("docker stats --no-stream --format \"{{json .}}\"");
    json j_info = json::parse(info_str);
    
    auto long_id = j_info["Container"];
    auto short_id = j_info["ID"];
    auto name = j_info["Name"];
    auto cpuPerc = j_info["CPUPerc"];
    auto memPerc = j_info["MemPerc"];
    auto memUsage = j_info["MemUsage"];
    auto NetIO = j_info["NetIO"];
    
    std::cout << cpuPerc << '\n';

    std::string inspect = getInfo("docker inspect bd372ab98153"); // 返回标准json
    json j = json::parse(inspect);
    json obj = j[0];
    auto ip = obj["Args"];
    std::cout << ip << '\n';
    // docker inspect bd372ab98153(container_ID)
    // docker top <container_ID> 容器内部进程监控
    return 0;

    // GUI

    // QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
    // return QCoreApplication::exec();
}
