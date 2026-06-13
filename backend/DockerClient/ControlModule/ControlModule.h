#pragma once
#include <qobject.h>
#include <qtmetamacros.h>
#include <QString>
#include <QNetworkAccessManager>

class ControlModule : public QObject{
    Q_OBJECT
    QNetworkAccessManager manager;
public:
    QString requestDockerAPI(const QString& method, const QString& endpoint);
    bool controlContainer(const QString& id, const QString &action);
    bool start(const QString& id);
    bool stop(const QString& id);
    bool restart(const QString& id);
    bool pause(const QString& id);
    bool unpause(const QString& id);
};