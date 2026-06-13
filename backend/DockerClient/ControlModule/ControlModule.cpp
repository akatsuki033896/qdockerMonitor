#include "ControlModule.h"

#include <qeventloop.h>

#include <QNetworkReply>

// request docker api from port 2375 via HTTP
QString ControlModule::requestDockerAPI(const QString& method,
                                        const QString& endpoint) {
    QNetworkRequest request(QUrl("http://127.0.0.1:2375" + endpoint));
    QNetworkReply* reply = nullptr;
    if (method == "GET") {
        reply = manager.get(request);
    } else if (method == "POST") {
        reply = manager.post(request, QByteArray());
    } else {
        qDebug() << "[ERROR] unsupported method";
        return "";
    }

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "[ERROR] docker api error: " << reply->errorString();
        delete reply;
        return "";
    }
    QString result = QString::fromUtf8(reply->readAll());
    reply->deleteLater();  // 异步删除
    return result;
}

bool ControlModule::controlContainer(const QString& id, const QString& action) {
    if (id.isEmpty()) {
        return false;
    }
    QString output =
        requestDockerAPI("POST", "/containers/" + id + "/" + action);
    return !output.isNull();
}

bool ControlModule::start(const QString& id) {
    return controlContainer(id, "start");
}

bool ControlModule::stop(const QString& id) {
    return controlContainer(id, "stop");
}

bool ControlModule::restart(const QString& id) {
    return controlContainer(id, "restart");
}

bool ControlModule::pause(const QString& id) {
    return controlContainer(id, "pause");
}

bool ControlModule::unpause(const QString& id) {
    return controlContainer(id, "unpause");
}