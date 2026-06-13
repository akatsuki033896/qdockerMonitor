//
// Created by akatsuki on 2026/6/6.
//

#ifndef SYSTEMMONITOR_DOCKERCLIENT_H
#define SYSTEMMONITOR_DOCKERCLIENT_H
#include <qnetworkaccessmanager.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <QString>
#include "container.h"
#include <QNetworkAccessManager>

class DataAcquireModule : public QObject {
    Q_OBJECT
    QNetworkAccessManager manager;
public:
    DataAcquireModule() = default;
    QString requestDockerAPI(const QString& method, const QString& endpoint);
    bool getInspect(Container& c);
    bool getStat(Container& c);
    bool getProcess(Container& c);
    DataAcquireModule(const DataAcquireModule&) = delete;
    DataAcquireModule& operator=(const DataAcquireModule&) = delete;
};

#endif //SYSTEMMONITOR_DOCKERCLIENT_H
