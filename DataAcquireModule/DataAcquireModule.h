//
// Created by akatsuki on 2026/6/6.
//

#ifndef SYSTEMMONITOR_DOCKERCLIENT_H
#define SYSTEMMONITOR_DOCKERCLIENT_H
#include <QString>
#include "container.h"

class DataAcquireModule {
public:
    DataAcquireModule() = default;
    QString requestDockerAPI(const QString& method, const QString& endpoint);
    bool getInspect(Container& c);
    bool getStat(Container& c);
    bool getProcess(const Container& c);
    DataAcquireModule(const DataAcquireModule&) = delete;
    DataAcquireModule& operator=(const DataAcquireModule&) = delete;
};

#endif //SYSTEMMONITOR_DOCKERCLIENT_H
